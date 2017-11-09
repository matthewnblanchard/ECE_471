#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {

        char *ANSI[7] = {       // ANSI escape codes for color
                "\x1b[0m",      // RESET
                "\x1b[31m",     // RED
                "\x1b[32m",     // GREEN
                "\x1b[33m",     // YELLOW
                "\x1b[34m",     // BLUE
                "\x1b[35m",     // MAGENTA
                "\x1b[36m"};    // CYAN

        char *msg = "ECE471 I can't think of a message\n";      // Printable message
        int num = 0;            // Number of lines to print

        // No more than one argument. If no integer is passed, print 20 lines. Otherwise, the passed integer is the 
        // number of lines to print.
        if (argc > 2) {
                printf("Too many arguments, expected one positive integer");
                return 1;
        } else if (argc == 2) {
                num = atoi(argv[1]);
                if (num <= 0) {
                        printf("Invalid argument. Expected one positive integer");
                        return 2;
                }
        } else if (argc == 1) {
                num = 20;
        }

        int i, j = 0;      // Loop indices
        for (i = 1; i <= num; i++) {
                printf(ANSI[0]);        // Reset color
                printf("#%d: ", i);     // Print line number

                for (j = 0; j < strlen(msg); j++){
                        printf(ANSI[(j % 6) + 1]);      // Cycle through ANSI color codes
                        printf("%c", msg[j]);           // Print message by character
                }
        }

	return 0;
}
