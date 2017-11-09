#include <stdio.h>

#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <string.h>
#include <errno.h>      

int gpio_export(char *num);                     // GPIO export function
int gpio_direction(char *num, char *dir);       // GPIO direction function 
int gpio_setvalue(char *num, char *val);        // GPIO value setting function

int main(int argc, char **argv) {

        if (gpio_export("18") < 0) {             // Enable GPIO 18
                return errno;
        }
        if (gpio_direction("18", "out") < 0) {   // Set GPIO 18 to output
                return errno;
        }

        // Blink with half second intervals
        while(1) {
                if (gpio_setvalue("18", "1") < 0) {
                       return 0;
                } 
                usleep(500000);         // 500,000 usec = 0.5 sec
                if (gpio_setvalue("18", "0") < 0) {
                        return 0;
                }
                usleep(500000);
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

// Name: gpio_value
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
