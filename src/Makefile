CFLAGS=-O2 -I${includedir}
LIBS=-lc -lemu

all:	e2part.exe e2pwd.exe e2cat.exe e2cp.exe e2cd2.exe e2ls.exe

e2part.exe:	e2part.o diskio.o part.o e2err.o
	$(CC) $(CFLAGS) -o e2part.exe e2part.o diskio.o part.o e2err.o $(LIBS)
#	coff2exe e2part

e2pwd.exe:	e2pwd.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	$(CC) $(CFLAGS) -o e2pwd.exe e2pwd.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a $(LIBS)
#	coff2exe e2pwd

e2cat.exe:	e2cat.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	$(CC) $(CFLAGS) -o e2cat.exe e2cat.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a $(LIBS)
#	coff2exe e2cat

e2cp.exe:	e2cp.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	$(CC) $(CFLAGS) -o e2cp.exe e2cp.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a $(LIBS)
#	coff2exe e2cp

e2cd2.exe:	e2cd2.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	$(CC) $(CFLAGS) -o e2cd2.exe e2cd2.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a $(LIBS)
#	coff2exe e2cd2

e2ls.exe:	e2ls.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	$(CC) $(CFLAGS) -o e2ls.exe e2ls.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a $(LIBS)
#	coff2exe e2ls

%.o:	%.c
	$(CC) $(CFLAGS) -I. -I.. -I../linux -c $<

clean:
	rm -f *.o
	rm -f e2part.exe
	rm -f e2pwd.exe
	rm -f e2cat.exe
	rm -f e2cp.exe
	rm -f e2cd2.exe
	rm -f e2ls.exe
	rm -f e2part
	rm -f e2pwd
	rm -f e2cat
	rm -f e2cp
	rm -f e2cd2
	rm -f e2ls
