CC = gcc
CFLAGS = -O2 -Wall
LFLAGS =


all:	temp_1wire temp_cool

###

temp_cool_c:	temp_cool_c.o
	$(CC) $(LFLAGS) -o temp_cool_c temp_cool_c.o

temp_cool_c.o:	temp_cool_c.c
	$(CC) $(CFLAGS) -c temp_cool_c.c

###

temp_1wire_c:	temp_1wire_c.o
	$(CC) $(LFLAGS) -o temp_1wire_c temp_1wire_c.o

temp_1wire_c.o:	temp_1wire_c.c
	$(CC) $(CFLAGS) -c temp_1wire_c.c


submit:	temp_1wire.c README Makefile temp_cool.c sample.sh
	tar -czvf hw8_submit.tar.gz Makefile temp_1wire.c temp_cool.c README sample.sh

clean:	
	rm -f *~ *.o temp_1wire temp_cool

