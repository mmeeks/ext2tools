/***************************************************************************
 * part.c - Routines for handling disk partitions
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/
 
#include <string.h>
#include <sys/types.h>
#include "genhd.h"
#include "part.h"
#include "diskio.h"
#include "ext2_fs.h"
#include "e2err.h"


struct firstsector fs, fs2;

/**********************************************************************
 * get_part() fetches 16 partition descriptors from the disk.
 *
 * Returns error code.
 **********************************************************************/

int
get_part(int disk, struct part_desc *pl)
{
	int i, next, err;
	long startsect;

	err=readdisk(disk, 0, 1, &fs);
	if (err) return err;

	if (fs.magic!=FIRST_SECT_MAGIC)
		return E2E_BADPART;
	
	memset(pl,0,sizeof(*pl)*16);
	for (i=0; i<4; i++) {
		if (fs.part[i].sys_ind) {
			pl[i].is_extended = fs.part[i].sys_ind==EXTENDED_PARTITION;
			pl[i].start = fs.part[i].start_sectlo+(fs.part[i].start_secthi<<16);
			pl[i].length = fs.part[i].nr_sectslo+(fs.part[i].nr_sectshi<<16);
			pl[i].parttype = fs.part[i].sys_ind;
		}
	}
	next=4;

	for (i=0; i<4; i++) {
		if (fs.part[i].sys_ind==EXTENDED_PARTITION) {
			startsect = pl[i].start;
			while (1) {
				if (next==16)
					return 0;

				err=readdisk(disk, startsect, 1, &fs2);
				if (err) return err;

				if (fs2.magic!=FIRST_SECT_MAGIC)
					return E2E_BADPART;

				pl[next].start = startsect + fs2.part[0].start_sectlo+(fs2.part[0].start_secthi<<16);
				pl[next].length = fs2.part[0].nr_sectslo+(fs2.part[0].nr_sectshi<<16);
				next++;
				if (fs2.part[1].sys_ind!=EXTENDED_PARTITION)
					break;
				startsect += fs2.part[1].start_sectlo+(fs2.part[1].start_secthi<<16);
			}
		}
	}
	return 0;
}


/**********************************************************************
 * getfstype finds the type of the file system on the partition.
 * It is not very good at it, but it identifies ext2 file systems,
 * which is the important thing.
 *
 * Returns the file system type or a negative error code.
 **********************************************************************/

int
getfstype(int disk, unsigned short parttype, unsigned long start)
{
	static union {
		u_char	bytes[512];
		u_short shorts[256];
		u_long	longs[128];
	} testblock;
	int err;

	if (parttype == PART_SWAP)
		return SWAP;

	if (parttype == PART_DOS32)
		return MSDOSFS;

	err=readdisk(disk, start, 1, &testblock);
	if (err) return -err;

#if 0
	if (strncmp(testblock.bytes+3,"MSDOS",5)==0) /* There must be a better way to do this */
		return MSDOSFS;
#endif

	err=readdisk(disk, start+2, 1, &testblock);
	if (err) return -err;

	if (testblock.shorts[28]==EXT2_SUPER_MAGIC)
		return EXT2FS;

	err=readdisk(disk, start+7, 1, &testblock);
	if (err) return -err;

	if (memcmp(testblock.bytes+0x1f6, "SWAP-SPACE", 10)==0)
		return SWAP;

	return 0;
}
