CC = gcc
CFLAGS = -O2 -Wall
LFLAGS =


all:	display_test display_final display_cool

###

display_test:	display_test.o
	$(CC) $(LFLAGS) -o display_test display_test.o

display_test.o:	display_test.c
	$(CC) $(CFLAGS) -c display_test.c


###

display_final:	display_final.o
	$(CC) $(LFLAGS) -o display_final display_final.o

display_final.o:	display_final.c
	$(CC) $(CFLAGS) -c display_final.c

###

display_cool:	display_cool.o
	$(CC) $(LFLAGS) -o display_cool display_cool.o

display_cool.o:	display_cool.c
	$(CC) $(CFLAGS) -c display_cool.c

###

submit:	display_test.c display_final.c display_cool.c README Makefile
	tar -czvf hw5_submit.tar.gz display_test.c display_final.c display_cool.c README Makefile


clean:	
	rm -f *~ *.o display_test display_final display_cool

