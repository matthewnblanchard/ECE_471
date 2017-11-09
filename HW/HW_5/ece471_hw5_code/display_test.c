#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// Display command defines
#define CMD_SYS_SET     (0x2 << 4)      // System set command
#define VAL_OSC_ON      (0x1 << 0)      // Oscillator on

#define CMD_DISP_SET    (0x8 << 4)      // Display set command
#define VAL_DISP_ON     (0x1 << 0)      // Display on
#define VAL_BLINK_OFF   (0x0 << 0)      // Blinking off

#define CMD_BRIGHT_SET  (0xE << 4)      // Brightness set command


int main(int argc, char **argv) {

	int fd;                         // File descriptor
	char i2c_device[]="/dev/i2c-1"; // I2C device file
	unsigned char buffer[17];       // Buffer 
        int i = 0;                      // Loop index

        // Open I2C device
	fd = open(i2c_device, O_RDWR);
        if (fd < 0 ) {
                printf("ERROR: Failed to open I2C device\n");
                return -1;
        }

        // Set slave address to 0x70 - the address of the display
        if (ioctl(fd, I2C_SLAVE, 0x70) < 0) {
                printf("Failed to set I2C slave address\n");
                return -2;
        }

        // Turn oscillator on
        buffer[0] = CMD_SYS_SET | VAL_OSC_ON;
        if (write(fd, buffer, 1) < 0) {
                printf("Failed toggle oscillator on\n");
                return -3;
        }

	// Turn display on, w/o blinking 
        buffer[0] = CMD_DISP_SET | VAL_DISP_ON | VAL_BLINK_OFF;
        if (write(fd, buffer, 1) < 0) {
                printf("Failed toggle display on\n");
                return -4;
        }

        // Set brightness to max
        buffer[0] = CMD_BRIGHT_SET | 0xF;
        if (write(fd, buffer, 1) < 0) {
                printf("Failed to set brightness\n");
                return -5;
        }

        // Write 1's to all segments
        buffer[0] = 0x0;       // Display Data Address Pointer
        for (i = 0;i < 16;i++) {
                buffer[1 + i] = 0xFF;   // 1 for each row
        }  
        write(fd, buffer, 17);  // Write to device

        // Close device
        close(fd);

	return 0;
}
