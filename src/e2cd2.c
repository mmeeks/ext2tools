/***************************************************************************
 * e2cd2.c - generate .BAT file for environment change
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "ext2_fs.h"
#include "ext2fs/ext2fs.h"
#include "ldisk.h"
#include "istat.h"
#include "e2err.h"

extern io_manager dos_io_manager;


/**********************************************************************
 * usage prints usage information and exits
 **********************************************************************/

void
usage()
{
	fprintf(stderr, "usage: e2cd directory\n");
	exit(1);
}


/**********************************************************************
 * main routine
 **********************************************************************/

main(int argc, char **argv)
{
	int err;
	char env[80], *pos1, *pos2;
	ext2_filsys fs;
	ino_t ino;
	struct ext2_inode e2ino;
	FILE *ofile;

	if (argc!=2)
		usage();

	/* Open file system */
	err = ext2fs_open(0, 0, 0, 0, dos_io_manager, &fs);
	if (err)
		e2_err("Cannot open ext2 file system",err);

	/* Lookup specified name */
	err = ext2fs_namei(fs, 2, cwdino, argv[1], &ino);
	if (err)
		e2_err("Cannot find file",err);

	/* Read specified inode */
	err = ext2fs_read_inode(fs, ino, &e2ino);
	if (err)
		e2_err("Cannot read inode information",err);

	/* Is it a directory? */
	if (!S_ISDIR(e2ino.i_mode)) {
		fprintf(stderr,"%s is not a directory\n",argv[1]);
		exit(1);
	}

	ofile=fopen("___e2cd.bat","w");
	if (!ofile) {
		fprintf(stderr,"Cannot create ___e2cd.bat\n");
		exit(1);
	}

	/* We know E2CWD exists, ext2fs_open already checked that */
	strcpy(env,getenv("E2CWD")); /* We are going to modify it, so we make a copy */
	pos1=strchr(env,':');
	pos2=strchr(pos1+1,':');
	if (pos2)
		*pos2=0;

	fprintf(ofile,"SET E2CWD=%s:%d\n",env,ino);
	fclose(ofile);
	
	return 0;
}
