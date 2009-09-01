/*
 * 	genhd.h Copyright (C) 1992 Drew Eckhardt
 *	Generic hard disk header file by  
 * 		Drew Eckhardt
 *
 *		<drew@colorado.edu>
 */
	
#define EXTENDED_PARTITION 5
	
struct partition {
	unsigned char boot_ind;		/* 0x80 - active */
	unsigned char head;		/* starting head */
	unsigned char sector;		/* starting sector */
	unsigned char cyl;		/* starting cylinder */
	unsigned char sys_ind;		/* What partition type */
	unsigned char end_head;		/* end head */
	unsigned char end_sector;	/* end sector */
	unsigned char end_cyl;		/* end cylinder */
	unsigned short start_sectlo;	/* starting sector counting from 0 */
	unsigned short start_secthi;	/* starting sector counting from 0 */
	unsigned short nr_sectslo;	/* nr of sectors in partition */
	unsigned short nr_sectshi;	/* nr of sectors in partition */
};

#define FIRST_SECT_MAGIC 0xaa55

struct firstsector {
	unsigned char    fill[0x1be];
	struct partition part[4];
	unsigned short   magic;		/* Must be FIRST_SECT_MAGIC */
};
