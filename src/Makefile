CFLAGS=-O0 -g -I${includedir} -I../ext2fs
LIBS=-lc -lemu
EXT2FS=$(wildcard ../ext2fs/*.o) $(wildcard ../e2p/*.o) $(wildcard ../et/*.o)

# all:	hello.exe e2part.exe e2pwd.exe e2cat.exe e2cp.exe e2cd2.exe e2ls.exe # e2dump.exe
all:	e2part.exe e2cat.exe e2cp.exe e2ls.exe


# hello.exe: hello.o
# 	$(CC) $(CFLAGS) -o hello.exe hello.o $(LIBS)

e2part.exe:	e2part.o diskio.o part.o e2err.o
	$(CC) $(CFLAGS) -o e2part.exe e2part.o diskio.o part.o e2err.o $(LIBS)

e2cat.exe:	e2cat.o diskio.o part.o ldisk.o e2err.o
	$(CC) $(CFLAGS) -o e2cat.exe e2cat.o diskio.o part.o ldisk.o e2err.o $(EXT2FS) $(LIBS)

e2cp.exe:	e2cp.o diskio.o part.o ldisk.o e2err.o
	$(CC) $(CFLAGS) -o e2cp.exe e2cp.o diskio.o part.o ldisk.o e2err.o $(EXT2FS) $(LIBS)

e2ls.exe:	e2ls.o diskio.o part.o ldisk.o e2err.o
	$(CC) $(CFLAGS) -o e2ls.exe e2ls.o diskio.o part.o ldisk.o e2err.o $(EXT2FS) $(LIBS)

# e2pwd.exe:	e2pwd.o diskio.o part.o ldisk.o e2err.o
#	$(CC) $(CFLAGS) -o e2pwd.exe e2pwd.o diskio.o part.o ldisk.o e2err.o $(EXT2FS) $(LIBS)

# e2cd2.exe:	e2cd2.o diskio.o part.o ldisk.o e2err.o
# 	$(CC) $(CFLAGS) -o e2cd2.exe e2cd2.o diskio.o part.o ldisk.o e2err.o $(EXT2FS) $(LIBS)

# e2dump.exe:	e2dump.o diskio.o part.o ldisk.o e2err.o
# 	$(CC) $(CFLAGS) -o e2dump.exe e2dump.o diskio.o part.o ldisk.o e2err.o $(EXT2FS) $(LIBS)

%.o:	%.c
	$(CC) $(CFLAGS) -I. -I.. -I../linux -c $<

clean:
	rm -f *.o
	rm -f *.exe
