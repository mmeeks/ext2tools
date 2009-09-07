/***************************************************************************
 * e2pwd - Prints current directory
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>

#include "ext2_fs.h"
#include "ext2fs/ext2fs.h"
#include "ldisk.h"
#include "istat.h"
#include "e2err.h"

extern io_manager dos_io_manager;

int aflag, dflag, iflag, lflag, tflag, rflag;

struct fileinfo {
	ino_t inode;
	unsigned char name[80];
	struct ext2_inode e2ino;
} *list;
int listix;
int maxlist;

char directory[256];
char dir2[256];

ino_t ino;

/**********************************************************************
 * myproc is a callback routine which is called once for each entry
 * in the directory
 **********************************************************************/

static int myproc(struct ext2_dir_entry *dirent,
				  int	offset,
				  int	blocksize,
				  char	*buf,
				  void	*private)
{
	if (dirent->inode == ino) {
		int real_len = dirent->name_len & 0xff;
		if (directory[0]==0) {
			strncpy(directory, dirent->name, real_len);
			directory[real_len]=0;
		}
		else {
			strncpy(dir2, dirent->name, real_len);
			dir2[real_len]=0;
			strcat(dir2, "/");
			strcat(dir2,directory);
			strcpy(directory,dir2);
		}
		return DIRENT_ABORT;
	}

	return 0;
}


/**********************************************************************
 * usage prints usage information and exits
 **********************************************************************/

void
usage()
{
	fprintf(stderr, "usage: e2pwd\n");
	exit(1);
}


/**********************************************************************
 * main routine
 **********************************************************************/

main(int argc, char **argv)
{
	int err, i;
	ext2_filsys fs;
	ino_t dotdotino;
	struct ext2_inode e2ino;
	char *filename;

	if (argc!=1)
		usage();

	/* Open file system */
	err = ext2fs_open(0, 0, 0, 0, dos_io_manager, &fs);
	if (err)
		e2_err("Cannot open ext2 file system",err);


	/* Loop until we have the root */
	for (ino=cwdino; ino!=2; ino=dotdotino) {
		/* Look up '..' */
		err = ext2fs_namei(fs, 2, ino, "..", &dotdotino);
		if (err)
			e2_err("Cannot find '..'",err);

		err = ext2fs_read_inode(fs, dotdotino, &e2ino);
		if (err)
			e2_err("Cannot read '..'", err);

		if (!S_ISDIR(e2ino.i_mode)) {
			fprintf(stderr,".. is not a directory\n");
			exit(1);
		}

		/* Loop through all directory entries */
		err = ext2fs_dir_iterate(fs, dotdotino, 0, 0, myproc, 0);
		if (err)
			e2_err("Cannot read directory",err);
	}

	printf("/%s\n",directory);
}		

