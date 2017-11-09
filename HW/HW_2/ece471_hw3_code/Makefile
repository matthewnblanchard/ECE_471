LD = ld 
AS = as 
ASFLAGS_THUMB = -mthumb-interwork
LDFLAGS_THUMB = --thumb-entry=_start

CC = gcc
CFLAGS = -O2 -Wall
LFLAGS =


all:	exit_asm hello_world hello_world.thumb hello_world.thumb2 hello_world.extra


exit_asm:	exit_asm.o
	$(LD) $(LDFLAGS) -o exit_asm exit_asm.o

exit_asm.o:	exit_asm.s
	$(AS) $(ASFLAGS) -o exit_asm.o exit_asm.s


hello_world:	hello_world.o
	$(LD) -o hello_world hello_world.o

hello_world.o:	hello_world.s
	$(AS) $(ASFLAGS) -o hello_world.o hello_world.s


hello_world.thumb:	hello_world.thumb.o
	$(LD) -o hello_world.thumb hello_world.thumb.o

hello_world.thumb.o:	hello_world.thumb.s
	$(AS) $(ASFLAGS) -o hello_world.thumb.o hello_world.thumb.s


hello_world.extra:	hello_world.extra.o
	$(LD) -o hello_world.extra hello_world.extra.o

hello_world.extra.o:	hello_world.extra.s
	$(AS) $(ASFLAGS) -o hello_world.extra.o hello_world.extra.s

hello_world.thumb2:	hello_world.thumb2.o
	$(LD) --thumb-entry=_start -o hello_world.thumb2 hello_world.thumb2.o

hello_world.thumb2.o:	hello_world.s
	$(AS) $(ASFLAGS) -mthumb -march=armv7-a -o hello_world.thumb2.o hello_world.s


submit:	hello_world.s hello_world.thumb.s hello_world.extra.s exit_asm.s Makefile README
	tar -czvf hw3_submit.tar.gz hello_world.s hello_world.thumb.s hello_world.extra.s exit_asm.s Makefile README

clean:	
	rm -f *~ *.o exit_asm hello_world hello_world.thumb hello_world.thumb2 hello_world.extra
