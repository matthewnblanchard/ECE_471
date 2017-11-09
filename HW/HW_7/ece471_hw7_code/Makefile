CC = gcc
CFLAGS = -O2 -Wall
LFLAGS =


all:	test_spi display_temp temp_cool

###

test_spi_c:	test_spi_c.o
	$(CC) $(LFLAGS) -o test_spi_c test_spi_c.o

test_spi_c.o:	test_spi_c.c
	$(CC) $(CFLAGS) -c test_spi_c.c

###

temp_cool_c:	temp_cool_c.o
	$(CC) $(LFLAGS) -o temp_cool_c temp_cool_c.o

temp_cool_c.o:	temp_cool_c.c
	$(CC) $(CFLAGS) -c temp_cool_c.c

###

display_temp_c:	display_temp_c.o
	$(CC) $(LFLAGS) -o display_temp_c display_temp_c.o

display_temp_c.o:	display_temp_c.c
	$(CC) $(CFLAGS) -c display_temp_c.c


submit:	display_temp.c test_spi.c README Makefile temp_cool.c
	tar -czvf hw7_submit.tar.gz Makefile display_temp.c test_spi.c temp_cool.c README

clean:	
	rm -f *~ *.o display_temp test_spi temp_cool

