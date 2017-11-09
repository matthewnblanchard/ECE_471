#include <stdio.h>

#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int gpio_export(char *num);                     // GPIO export function
int gpio_direction(char *num, char *dir);       // GPIO direction function 
int gpio_setvalue(char *num, char *val);        // GPIO value setting function
int gpio_readvalue(char *num);                  // Read value from GPIO
int debounce(char *num);                        // Software debouncing for GPIO pins

int main(int argc, char **argv) {

        if (gpio_export("17") < 0) {              // Enable GPIO 17
                return errno;
        }
        if (gpio_direction("17", "in") < 0) {     // Set GPIO 17 to input
                return errno;
        }
        if (gpio_export("18") < 0) {              // Enable GPIO 18
                return errno;
        }
        if (gpio_direction("18", "out") < 0) {    // Set GPIO 18 to output
                return errno;
        }

        while(1) {
                while(!debounce("17"));           // Wait until button is pressed
                printf("Button pressed!\n");    
                gpio_setvalue("18", "1");         // Turn on LED

                while(debounce("17"));            // Wait until button is released
                printf("Button released!\n");
                gpio_setvalue("18", "0");         // Turn off LED
        }

        return 0;
}

// Name: gpio_export
// Description: Export given GPIO pin 
// Inputs:
//      num: Number of the GPIO pin
// Return:
//      0 if successful, or a negative value if an error occured
int gpio_export(char *num) {

        int fd = 0;             // File descriptor

        // Open export file
        fd = open("/sys/class/gpio/export", O_WRONLY);
        if (fd < 0) {
                printf("ERROR %d: Failed to open GPIO export file ...\n", errno);
                return errno;
        }

        // Select given GPIO pin for export
        if (write(fd, num, strlen(num)) < 0) {
                printf("ERROR %d: Failed to export GPIO %s ...\n", errno, num);
                return errno;
        }

        // Close export file
        if (close(fd) < 0) {
                printf("ERROR %d: Failed to close GPIO export file ...\n", errno);
                return errno;
        }

        return 0;
}

// Name: gpio_direction
// Description: Set direction of given GPIO pin 
// Inputs:
//      num: Number of the GPIO pin
//      dir: Desired direction for GPIO pin
// Return:
//      0 if successful, or a negative value if an error occured
int gpio_direction(char *num, char *dir) {

        int fd = 0;     // File descriptor
        char path[40];  // Buffer for file path

        // Assemble file path
        if (sprintf(path, "/sys/class/gpio/gpio%s/direction", num) < 0) {
                printf("Error %d: failed to assemble GPIO %s direction file path ...\n", errno, num);
                return errno;
        }

        // Open direction file
        fd = open(path, O_WRONLY);
        if (fd < 0) {
                printf("ERROR %d: Failed to open GPIO export file ...\n", errno);
                return errno;
        }

        // Modify GPIO direction
        if (write(fd, dir, strlen(dir)) < 0) {
                printf("ERROR %d: Failed to modify GPIO %s direction ...\n", errno, num);
                return errno;
        }

        // Close export file
        if (close(fd) < 0) {
                printf("ERROR %d: Failed to close GPIO direction file ...\n", errno);
                return errno;
        }

        return 0;
}

// Name: gpio_setvalue
// Description: Set value of given GPIO pin 
// Inputs:
//      num: Number of the GPIO pin
//      val: Desired value for GPIO pin
// Return:
//      0 if successful, or a negative value if an error occured
int gpio_setvalue(char *num, char *val) {

        int fd = 0;     // File descriptor
        char path[40];  // Buffer for file path

        // Assemble file path
        if (sprintf(path, "/sys/class/gpio/gpio%s/value", num) < 0) {
                printf("Error %d: failed to assemble GPIO %s value file path ...\n", errno, num);
                return errno;
        }

        // Open value file
        fd = open(path, O_WRONLY);
        if (fd < 0) {
                printf("ERROR %d: Failed to open GPIO value file ...\n", errno);
                return errno;
        }

        // Modify GPIO value
        if (write(fd, val, strlen(val)) < 0) {
                printf("ERROR %d: Failed to modify GPIO %s value ...\n", errno, num);
                return errno;
        }

        // Close value file
        if (close(fd) < 0) {
                printf("ERROR %d: Failed to close GPIO value file ...\n", errno);
                return errno;
        }

        return 0;
}

// Name: gpio_readvalue
// Description: reads value of given GPIO pin 
// Inputs:
//      num: Number of the GPIO pin
// Return:
//      GPIO value if successful, or a negative value if an error occured
int gpio_readvalue(char *num) {

        int fd = 0;     // File descriptor
        char path[40];  // Buffer for file path
        char out[2];    // Buffer for value output

        // Assemble file path
        if (sprintf(path, "/sys/class/gpio/gpio%s/value", num) < 0) {
                printf("Error %d: failed to assemble GPIO %s value file path ...\n", errno, num);
                return errno;
        }

        // Open value file
        fd = open(path, O_RDONLY);
        if (fd < 0) {
                printf("ERROR %d: Failed to open GPIO value file ...\n", errno);
                return errno;
        }

        // Read GPIO value
        if (read(fd, out, 2) < 0) {
                printf("ERROR %d: Failed to modify GPIO %s value ...\n", errno, num);
                return errno;
        }

        // Close value file
        if (close(fd) < 0) {
                printf("ERROR %d: Failed to close GPIO value file ...\n", errno);
                return errno;
        }

        return atoi(out);
}

// Name: debounce
// Description: performs software debouncing for given GPIO pin. 
//      checks GPIO pin value until value is consistent for 4 consecutive 
//      polls, then returns the value
// Inputs:
//      num: GPIO number
// Return:
//      Final GPIO value
int debounce(char *num) {

        int last = -1;  // Last reading
        int cur = -1;   // Current reading
        int count = 0;  // Count of consecutive readings

        cur = gpio_readvalue(num);
        count++;

        // Read value and compare to last value until 4 consecutive matches are reached
        while (count < 4) {

                last = cur;
                cur = gpio_readvalue(num);

                if (cur == last) {
                        count++;
                } else {
                        count = 0;
                }

        }

        return cur;     // Return the last reading
}
