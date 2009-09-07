/***************************************************************************
 * e2ls - DOS ls program for ext2 file systems
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
	ino_t	inode;
	unsigned char	name[80];
	struct ext2_inode e2ino;
} *list;
int listix;
int maxlist;

void outino(struct fileinfo *);


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
	int real_len = dirent->name_len & 0xff;
	int type = dirent->name_len >> 8;

	if (!list) {
		list = malloc(100*sizeof(struct fileinfo));
		if (!list)
			return E2E_BADMEM;
		maxlist = 100;
	}

	if (dirent->name[0]=='.' && !aflag)
		return 0;

	list[listix].inode = dirent->inode;
	strncpy (list[listix].name, dirent->name, real_len);
	list[listix].name[real_len] = '\0';

	listix++;
	if (listix == maxlist) {
		list = realloc(list, (maxlist+100) * sizeof(struct fileinfo));
		if (!list) {
			fprintf(stderr,"Cannot allocate memory\n");
			return DIRENT_ABORT;
		}
		maxlist += 100;
	}
	return 0;
}


/**********************************************************************
 * compare is used by qsort() to compare to list entries
 **********************************************************************/

int
compare(const void *ee1, const void *ee2)
{
	int res;
	const struct fileinfo *e1 = ee1, *e2 = ee2;

	if (tflag)
		res = e1->e2ino.i_mtime < e2->e2ino.i_mtime ? 1 : -1;
	else
		res = strcmp(e1->name, e2->name);

	return rflag ? -res : res;
}


/**********************************************************************
 * usage prints usage information and exits
 **********************************************************************/

void
usage()
{
	fprintf(stderr, "usage: e2ls [-adiltr] [file]\n");
	exit(1);
}

static int
dump_dir_ent (struct ext2_dir_entry *dirent,
	      int offset, int blocksize,
	      char *buf, void *priv_data)
{
    char name[256+1];
    int real_len = dirent->name_len & 0xff;
    int type = dirent->name_len >> 8;
    strncpy (name, dirent->name, real_len);
    name[real_len] = '\0';
    fprintf (stderr, "dir entry '%s'\n", name);

    return 0;
}

/**********************************************************************
 * main routine
 **********************************************************************/

main(int argc, char **argv)
{
	int err, i, c;
	ext2_filsys fs;
	ino_t ino;
	struct ext2_inode e2ino;
	char *filename;

	opterr = 0;
	while ((c=getopt(argc, argv, "adiltr")) != -1) {
		switch (c) {
         case 'a': aflag++; break;
		 case 'd': dflag++; break;
         case 'i': iflag++; break;
         case 'l': lflag++; break;
         case 't': tflag++; break;
         case 'r': rflag++; break;
		 case '?':
		case 'h':
		  usage();
		}
	}

	if (argc==optind)
		filename = ".";
	else if (argc!=optind+1)
		usage();
	else
		filename = argv[optind];

	/* Open file system */
	err = ext2fs_open(0, 0, 0, 0, dos_io_manager, &fs);
	if (err)
		e2_err("Cannot open ext2 file system",err);

#if 0
	{
	  struct ext2_inode ino;
	  ext2fs_read_inode (fs, 2 /* root inode */, &ino);
	  if (!S_ISDIR (ino.i_mode))
	    fprintf (stderr, "error: root dir is not a dir !(0x%x) blocks %d flags 0x%x\n",
		     ino.i_mode, ino.i_blocks, ino.i_flags);
	  ext2fs_dir_iterate (fs, 2, 0, NULL, dump_dir_ent, NULL);
	}
#endif

	/* Lookup specified name */
	err = ext2fs_namei(fs, 2, cwdino, filename, &ino);
	if (err)
		e2_err("Cannot find file",err);

	/* Read specified inode */
	err = ext2fs_read_inode(fs, ino, &e2ino);
	if (err)
		e2_err("Cannot read inode information", err);

	/* Is it a directory? */
	if (!S_ISDIR(e2ino.i_mode) || dflag) {
		struct fileinfo fi;
		fi.inode = ino;
		strcpy(fi.name, filename);
		fi.e2ino = e2ino;
		outino(&fi);
	}
	else {
		/* Loop through all directory entries */
		err = ext2fs_dir_iterate(fs, ino, 0, 0, myproc, 0);
		if (err)
			e2_err("Cannot read directory",err);

		if (lflag || tflag)
			for (i=0; i<listix; i++) {
				err = ext2fs_read_inode(fs, list[i].inode, &list[i].e2ino);
				if (err)
					e2_err("Cannot read file information",err);
			}


		qsort(list, listix, sizeof(list[0]), compare);

		for (i=0; i<listix; i++)
			outino(&list[i]);
	}
}		


/**********************************************************************
 * outino writes one line of directory information
 **********************************************************************/

void
outino(struct fileinfo *fi)
{
	char *mtimestring;
	time_t now;

	time(&now);

	if (iflag)
		printf("%5d ",fi->inode);

	if (lflag) {
		if (S_ISDIR(fi->e2ino.i_mode))
			printf("d");
		else if (S_ISREG(fi->e2ino.i_mode))
			printf("-");
		else if (S_ISLNK(fi->e2ino.i_mode))
			printf("l");
		else if (S_ISCHR(fi->e2ino.i_mode))
			printf("c");
		else if (S_ISBLK(fi->e2ino.i_mode))
			printf("b");
		else if (S_ISFIFO(fi->e2ino.i_mode))
			printf("p");
		else if (S_ISSOCK(fi->e2ino.i_mode))
			printf("s");
		else
			printf("?");

		printf("%c%c%c%c%c%c%c%c%c ",
			   		fi->e2ino.i_mode&S_IRUSR ? 'r' : '-',
			   		fi->e2ino.i_mode&S_IWUSR ? 'w' : '-',
			   		fi->e2ino.i_mode&S_IXUSR ? 'x' : '-',
			   		fi->e2ino.i_mode&S_IRGRP ? 'r' : '-',
			   		fi->e2ino.i_mode&S_IWGRP ? 'w' : '-',
			   		fi->e2ino.i_mode&S_IXGRP ? 'x' : '-',
			   		fi->e2ino.i_mode&S_IROTH ? 'r' : '-',
			   		fi->e2ino.i_mode&S_IWOTH ? 'w' : '-',
			   		fi->e2ino.i_mode&S_IXOTH ? 'x' : '-');

		printf("%2d  %4d %4d ",
			   fi->e2ino.i_links_count, fi->e2ino.i_uid, fi->e2ino.i_gid);

		if (S_ISCHR(fi->e2ino.i_mode) || S_ISBLK(fi->e2ino.i_mode))
			printf("%3d, %3d ", fi->e2ino.i_block[0]>>8, fi->e2ino.i_block[0]&0xff);
		else
			printf("%8d ", fi->e2ino.i_size);

		mtimestring = ctime(&fi->e2ino.i_mtime);

		if (fi->e2ino.i_mtime < now-6*30*24*60*60) /* File more than 6 months old */
			printf("%.6s  %.4s ",mtimestring+4, mtimestring+20);
		else
			printf("%.12s ",mtimestring+4);
	}

	printf("%s",fi->name);
	
	if (lflag) {
		if (S_ISLNK(fi->e2ino.i_mode))
			printf(" -> %s",(char*)fi->e2ino.i_block);
	}
	printf("\n");
}

