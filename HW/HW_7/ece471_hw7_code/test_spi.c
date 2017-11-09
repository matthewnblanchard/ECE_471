#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define VREF 3.3
#define TEMP_CONV(X) ((float)X * (float)VREF) / (float)1024

int main(int argc, char **argv) {

        int spi_fd = 0;                         // File descriptor
        char spi_mode = SPI_MODE_0;             // SPI mode
        int result = 0;                         // Result of IOCTL call
        struct spi_ioc_transfer spi_ch0;        // SPI transfer struct for channel 0
        memset(&spi_ch0, 0, sizeof(spi_ch0));    // Initialize tranfer struct to 0's
        struct spi_ioc_transfer spi_ch1;        // SPI transfer struct for channel 1
        memset(&spi_ch1, 0, sizeof(spi_ch1));    // Initialize transfer struct to 0's
        char tx_ch0[3] = {0x01, 0x80, 0x00};    // Single ended channel 0 read
        char tx_ch1[3] = {0x01, 0x90, 0x00};    // Single ended channel 1 read
        char rx_ch0[3];                         // Channel 0 response buffer
        memset(rx_ch0, 0, 3);                   // Initialize buffer to 0's
        char rx_ch1[3];                         // Channel 1 response buffer
        memset(rx_ch1, 0, 3);                   // Initialize buffer to 0's
        int temp = 0;                           // Temperature reading

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
        spi_ch0.tx_buf = (unsigned long)tx_ch0;
        spi_ch0.rx_buf = (unsigned long)rx_ch0;
        spi_ch0.len = 3;                // Size of rx buffer
        spi_ch0.delay_usecs = 0;        // No delay
        spi_ch0.speed_hz = 100000;      // 500 kHz
        spi_ch0.bits_per_word = 8;      // 8 bit words

        spi_ch1.tx_buf = (unsigned long)tx_ch1;
        spi_ch1.rx_buf = (unsigned long)rx_ch1;
        spi_ch1.len = 3;                // Size of rx buffer
        spi_ch1.delay_usecs = 0;        // No delay
        spi_ch1.speed_hz = 100000;      // 500 kHz
        spi_ch1.bits_per_word = 8;      // 8 bit words

        // Read temperatures and print every second
        while(1) {

                result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi_ch0);   // Read channel 0
                if (result < 0) {
                        printf("ERROR %d: Failed to communicate over SPI\n", errno);
                        return -4;
                }
                result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi_ch1);   // Read channel 1
                if (result < 0) {

                        printf("ERROR %d: Failed to communicate over SPI\n", errno);
                        return -5;
                }

                temp = 0;
                temp |= (*(char *)(spi_ch0.rx_buf + 1) & 0x03) << 8;    // Lower two bits of second byte are bit 9,8 of temp
                temp |= *(char *)(spi_ch0.rx_buf + 2);                  // Last byte is bits 7-0 of temp
                printf("Channel 0 voltage: %f\n", TEMP_CONV(temp));
                
                temp = 0;
                temp |= (*(char *)(spi_ch1.rx_buf + 1) & 0x03) << 8;    // Lower two bits of second byte are bit 9,8 of temp
                temp |= *(char *)(spi_ch1.rx_buf + 2);                  // Last byte is bits 7-0 of temp
                printf("Channel 1 voltage: %f\n", TEMP_CONV(temp));

                usleep(1000000);
        }

	return 0;
}
