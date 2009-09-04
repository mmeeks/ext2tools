CFLAGS=-O0 -g -I${includedir}

all:	alloc.o badblock.o bb_inode.o bitmaps.o bitops.o block.o \
     chk_desc.o closefs.o expandir.o freefs.o get_path.o \
     init.o inline.o inode.o link.o llseek.o mkdir.o namei.o newdir.o \
     openfs.o read_bb.o read_bbf.o rw_bitm.o
#	$(CC) -static -o ext2fs.a *.o

%.o:	%.c
	$(CC) $(CFLAGS) -I. -I.. -c $<

clean:
	rm -f *.o
	rm -f *.a
