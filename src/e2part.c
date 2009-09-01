/***************************************************************************
 * e2part - List hard disk partitions
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>

#include "e2err.h"
#include "part.h"
#include "diskio.h"
#include "ldisk.h"
#include "ext2_fs.h"




/**********************************************************************
 * usage prints usage information and exits
 **********************************************************************/

void
usage()
{
	fprintf(stderr, "usage: e2part disk_number\n");
	exit(1);
}



/**********************************************************************
 * main routine
 **********************************************************************/


main(int argc, char **argv)
{
	int i, err, fstype, diskno;
	struct part_desc pl[16];

	if (argc!=2)
		usage();

	diskno=atoi(argv[1]);
	if (!(diskno&0x80)) {
		fprintf(stderr,"Disk %d is not a hard disk\n",diskno);
		exit(1);
	}

	err=get_part(diskno, pl);
	if (err)
		e2_err("Cannot read disk partitions",err);

	printf("PARTITION  LENGTH  FILE SYSTEM TYPE\n");

	for (i=0; i<16; i++)
		if (pl[i].start && !pl[i].is_extended) {
			fstype=getfstype(diskno, pl[i].parttype, pl[i].start);
			if (fstype<0)
				e2_err("Disk error",-fstype);
			
			printf("%7d  %5d MB  ", i+1, (pl[i].length + 1024)/2048);
								/* 1024 was added above to make division round instead
								   of truncate */
			switch (fstype) {
			 case EXT2FS:
				printf("Linux ext2\n");
				break;

			 case MSDOSFS:
				printf("MS-DOS (FAT)\n");
				break;

			 case SWAP:
				printf("Linux swap space\n");
				break;

			 default:
				printf("Unknown\n");
				break;
			}
		}
	return 0;
}
