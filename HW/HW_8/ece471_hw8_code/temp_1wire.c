#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define SENSOR_NAME "/sys/bus/w1/devices/28-000005aaf7ed/w1_slave"

static double read_temp(char *filename) {

	double result=0.0;      // Read temperature
        int fd = 0;             // File descriptor
        char out_1[256];        // Output buffer 1
        char out_2[256];        // Output buffer 2

        // Open device files
        fd = open(SENSOR_NAME, O_RDONLY);
        if (fd < 0) {
                printf("ERROR: Failed to open file\n");
                return -1;
        }

        // Read sensor output
        fscanf("%s %s", out_1, out_2);
        printf("%s\n%s\n", out_1, out_2);

	return result;
}

int main(int argc, char **argv) {

	double temp1;

	while(1) {

		temp1=read_temp(SENSOR_NAME);
		printf("%.2lfC\n",temp1);
		sleep(1);
	}

	return 0;
}
