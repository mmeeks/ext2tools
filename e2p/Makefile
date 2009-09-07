CFLAGS=-O0 -g -I${includedir} -DHAVE_ERRNO_H -DHAVE_SYS_STAT_H -DHAVE_UNISTD_H

all:	     $(patsubst %.c,%.o,$(wildcard *.c))

%.o:	%.c
	$(CC) $(CFLAGS) -I. -I.. -c $<

clean:
	rm -f *.o
	rm -f *.a
