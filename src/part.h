/***************************************************************************
 * part.c - Routines for handling disk partitions
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/

/* Partition descriptor block */
struct part_desc {
	unsigned long	start;		 /* Starting sector number */
	unsigned long	length;		 /* Sector count */
	unsigned long	is_extended; /* Is extended partition */
	unsigned short	parttype;	 /* Partition type */
};

/* File system type */
#define	EXT2FS	1
#define MSDOSFS	2
#define	SWAP	3

/* Partition types */
#define PART_DOS32	6
#define PART_EXT2	0x83
#define PART_SWAP	0x82
	
extern int get_part(int disk, struct part_desc *pl);
extern int getfstype(int disk, unsigned short parttype, unsigned long start);
