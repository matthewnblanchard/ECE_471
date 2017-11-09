CC = gcc
CFLAGS = -O2 -Wall
LFLAGS =


all:	i2c-bitbang i2c-cool

###

i2c-bitbang:	i2c-bitbang.o
	$(CC) $(LFLAGS) -o i2c-bitbang i2c-bitbang.o

i2c-bitbang.o:	i2c-bitbang.c
	$(CC) $(CFLAGS) -c i2c-bitbang.c

###

i2c-cool:	i2c-cool.o
	$(CC) $(LFLAGS) -o i2c-cool i2c-cool.o

i2c-cool.o:	i2c-cool.c
	$(CC) $(CFLAGS) -c i2c-cool.c


submit:	i2c-bitbang.c i2c-cool.c README Makefile
	tar -czvf hw6_submit.tar.gz i2c-bitbang.c i2c-cool.c README Makefile

clean:	
	rm -f *~ *.o i2c-cool i2c-bitbang

