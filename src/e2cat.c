/***************************************************************************
 * e2cp.c - Copy an ext2 file to standard out
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "ext2_fs.h"
#include "ext2fs/ext2fs.h"
#include "ldisk.h"
#include "istat.h"
#include "e2err.h"

extern io_manager dos_io_manager;

char *blockbuf;

#define MIN(a,b) ((a)<(b) ? (a) : (b))

/**********************************************************************
 * usage prints usage information and exits
 **********************************************************************/

void
usage()
{
	fprintf(stderr, "usage: e2cat [-bt] file\n");
	exit(1);
}


/**********************************************************************
 * myproc is called once for every block in the file
 **********************************************************************/

static int myproc(ext2_filsys fs,
				  blk_t *blocknr,
				  int blockcnt,
				  void *private)
{
	int err;
	struct ext2_inode *ino = private;
	static nextblock = 0;

	if (blockcnt < 0)			/* We're reading an inderect block */
		return 0;

	if (nextblock+1 < blockcnt)
		memset(blockbuf, 0, fs->blocksize);

	while (nextblock++ < blockcnt)
		if (fwrite(blockbuf, fs->blocksize, 1, stdout) < 0) {
			fprintf(stderr,"Cannot write destination file\n");
			return BLOCK_ABORT;
		}

	err = io_channel_read_blk(fs->io, *blocknr, 1, blockbuf);
	if (err) {
		fprintf(stderr,"Cannot read source file\n");
		return BLOCK_ABORT;
	}

	if (fwrite(blockbuf, MIN(fs->blocksize, ino->i_size - blockcnt*fs->blocksize), 1, stdout) < 0) {
		fprintf(stderr,"Cannot write destination file\n");
		return BLOCK_ABORT;
	}
	
	return 0;
}


/**********************************************************************
 * main routine
 **********************************************************************/

main(int argc, char **argv)
{
	int err, c;
	char *src;
	ext2_filsys fs;
	ino_t ino;
	struct ext2_inode e2ino;

	opterr = 0;
	while ((c=getopt(argc, argv, "bt")) != -1) {
		switch (c) {
         case 'b': setmode(1, O_BINARY); break;
		 case 't': setmode(1, O_TEXT); break;
		 case '?': usage();
		}
	}

	if (argc!=optind+1)
		usage();

	src = argv[optind];


	/* Open file system */
	err = ext2fs_open(0, 0, 0, 0, dos_io_manager, &fs);
	if (err)
		e2_err("Cannot open ext2 file system",err);

	/* Allocate file buffer */
	blockbuf = malloc(fs->blocksize);
	if (!blockbuf) {
		fprintf(stderr,"Cannot malloc buffer\n");
		exit(1);
	}

	/* Lookup specified name */
	err = ext2fs_namei(fs, 2, cwdino, src, &ino);
	if (err)
		e2_err("Cannot find source file",err);

	/* Read specified inode */
	err = ext2fs_read_inode(fs, ino, &e2ino);
	if (err)
		e2_err("Cannot read inode information",err);

	/* Is it a regular file? */
	if (!S_ISREG(e2ino.i_mode)) {
		fprintf(stderr,"%s is not a regular file\n",src);
		exit(1);
	}

	err = ext2fs_block_iterate(fs, ino, 0, 0, myproc, &e2ino);
	if (err)
		e2_err("Cannot read source file",err);

	return 0;
}
