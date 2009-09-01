CC=gcc
CFLAGS=-O2 -I${includedir}

all:	e2part.exe e2pwd.exe e2cat.exe e2cp.exe e2cd2.exe e2ls.exe

e2part.exe:	e2part.o diskio.o part.o e2err.o
	$(CC) $(CFLAGS) -o e2part e2part.o diskio.o part.o e2err.o
	coff2exe e2part

e2pwd.exe:	e2pwd.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	$(CC) $(CFLAGS) -o e2pwd e2pwd.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	coff2exe e2pwd

e2cat.exe:	e2cat.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	$(CC) $(CFLAGS) -o e2cat e2cat.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	coff2exe e2cat

e2cp.exe:	e2cp.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	$(CC) $(CFLAGS) -o e2cp e2cp.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	coff2exe e2cp

e2cd2.exe:	e2cd2.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	$(CC) $(CFLAGS) -o e2cd2 e2cd2.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	coff2exe e2cd2

e2ls.exe:	e2ls.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	$(CC) $(CFLAGS) -o e2ls e2ls.o diskio.o part.o ldisk.o msdos_io.o e2err.o ../ext2fs/ext2fs.a
	coff2exe e2ls

%.o:	%.c
	$(CC) $(CFLAGS) -I. -I.. -I../linux -c $<

clean:
	del *.o
	del e2part.exe
	del e2pwd.exe
	del e2cat.exe
	del e2cp.exe
	del e2cd2.exe
	del e2ls.exe
	del e2part
	del e2pwd
	del e2cat
	del e2cp
	del e2cd2
	del e2ls
