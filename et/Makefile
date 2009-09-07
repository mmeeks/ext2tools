CFLAGS=-O0 -g -I${includedir}

all:	     $(patsubst %.c,%.o,$(wildcard *.c))

%.o:	%.c
	$(CC) $(CFLAGS) -I. -I.. -c $<

clean:
	rm -f *.o
	rm -f *.a
