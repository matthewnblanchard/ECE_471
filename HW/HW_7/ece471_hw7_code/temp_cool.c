#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define VREF 3.3
#define VOLT_CONV(X)    ((float)X * (float)VREF) / (float)1024
#define TEMP_CONV(X)    ((100 * (float)X) - 50)
#define F_CONV(X)       (((float)X * 1.8) + 32) 

int main(int argc, char **argv) {

        int spi_fd = 0;                         // File descriptor
        char spi_mode = SPI_MODE_0;             // SPI mode
        int result = 0;                         // Result of IOCTL call
        struct spi_ioc_transfer spi_ch2;        // SPI transfer struct for channel 2
        memset(&spi_ch2, 0, sizeof(spi_ch2));   // Initialize tranfer struct to 0's
        char tx_ch2[3] = {0x01, 0xA0, 0x00};    // Single ended channel 2 read
        char rx_ch2[3];                         // Channel 2 response buffer
        memset(rx_ch2, 0, 3);                   // Initialize buffer to 0's
        int data = 0;                           // Raw data reading
        float volt = 0;                         // Voltage reading 
        float temp = 0;                         // Temperature
        int temp_num = 0;                       // Number of temperature readings
        float temp_avg = 0;                     // Average temperature
        float temp_max = 0;                     // Maximum temperature
        float temp_min = 999999;                // Minimum temperature

        // Open SPI device 
	spi_fd = open("/dev/spidev0.0", O_RDWR);
        if (spi_fd < 0) {
                printf("ERROR: Failed to open SPI device\n");
                return -1;
        }

	// Set SPI mode to 0 for read and write
        result = ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode);
        if (result < 0) {
                printf("ERROR: Failed to set SPI mode\n");
                return -2;
        }
        result = ioctl(spi_fd, SPI_IOC_RD_MODE, &spi_mode);
        if (result < 0) {
                printf("ERROR: Failed to set SPI mode\n");
                return -3;
        }
        
        // Populate SPI transfer structures
        spi_ch2.tx_buf = (unsigned long)tx_ch2;
        spi_ch2.rx_buf = (unsigned long)rx_ch2;
        spi_ch2.len = 3;                // Size of rx buffer
        spi_ch2.delay_usecs = 0;        // No delay
        spi_ch2.speed_hz = 100000;      // 500 kHz
        spi_ch2.bits_per_word = 8;      // 8 bit words

        // Read temperatures and print every second
        while(1) {

                result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi_ch2);   // Read channel 2
                if (result < 0) {
                        printf("ERROR %d: Failed to communicate over SPI\n", errno);
                        return -4;
                }

                data = 0;
                data |= (*(char *)(spi_ch2.rx_buf + 1) & 0x03) << 8;    // Lower two bits of second byte are bit 9,8 of voltage
                data |= *(char *)(spi_ch2.rx_buf + 2);                  // Last byte is bits 7-0 of voltage
                volt = VOLT_CONV(data);                                 // Convert to voltage
                temp = TEMP_CONV(volt);                                 // Convert to celsius

                // Update statistics
                temp > temp_max ? temp_max = temp : 0;  // Update max
                temp < temp_min ? temp_min = temp : 0;  // Update min
                if (temp_num == 0) {
                        temp_avg = temp;
                } else {
                        temp_avg = ((temp_avg * temp_num) + temp) / (temp_num + 1);     // Update average 
                }
                temp_num++;
                printf("Temperature: %f degrees celsius, %f degrees fahrenheit\n", temp, F_CONV(temp));

                // Every 5th reading, print stats
                if ((temp_num % 5) == 0) {
                        printf("------ Statistics: avg = %fC/%fF, max = %fC/%fF, min = %fC/%fF ------\n",
                                temp_avg, F_CONV(temp_avg), temp_max, F_CONV(temp_max), temp_min, F_CONV(temp_min));
                }

                usleep(1000000);

        }

	return 0;
}
