#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define SENSOR_NAME "/sys/bus/w1/devices/28-000005aaf7ed/w1_slave"

static double read_temp(char *filename) {

	double result=0.0;

	/* Your code here */

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
