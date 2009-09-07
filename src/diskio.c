/***************************************************************************
 * diskio.c - Basic disk I/O routines
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/
 
#include <bios.h>
#include <stdio.h>
#include <sys/types.h>
#include "diskio.h"

static int cyls, heads, sects;

/**********************************************************************
 * getdiskparm finds the physical disk paramters
 **********************************************************************/

static int
getdiskparm(int disk)
{
	int i;
	u_char buf[4];

	i=biosdisk(8,disk,0,0,0,0,&buf);

	if (i!=0)
		return i;

	sects = buf[0] & 0x3f;
	cyls = ((buf[0] & 0xc0 ) << 2) + buf[1];
	heads = buf[3] + 1;
	return 0;
}

static void
hexdump_sect (FILE *file, unsigned char *data)
{
  int i, j;

  for (i = 0; i < 512/16; i++) {
    for (j = 0; j < 16; j++)
      fprintf (stderr, "%2x ", data[i*16+j]);
    fprintf (stderr, "| ");
    for (j = 0; j < 16; j++) {
      unsigned char c = data[i*16+j];
      fprintf (stderr, "%c", c >= 0x30 && c < 0x80 ? c : '.');
    }
    fprintf (stderr, "\n");
  }
  fscanf ("%c", stdin, &i);
}


/**********************************************************************
 * readdisk read a number of 512 byte sectors from the physical disk
 **********************************************************************/

int
readdisk(int disk, int start, int nsect, void *buf)
{
	int cyl, head, sect, err;

	if (sects==0) {
		err=getdiskparm(disk);
		if (err) return err;
	}

	sect = start % sects + 1;
	start /= sects;
	head = start % heads;
	cyl = start / heads;

	err = biosdisk(2, disk, head, cyl, sect, nsect, buf);
	if (err==6)
		/* Disk changed - try once more */
		err = biosdisk(2, disk, head, cyl, sect, nsect, buf);
	if (err != 0)
	  fprintf (stderr, "Error reading sector 0x%x\n", err);

	{
	  static FILE *log = stderr;
	  int i;

	  if (!log)
	    fopen ("log.txt", "w");
	  for (i = 0; i < nsect; i++) {
	    fprintf (log, "Dump sector 0x%x:\n", sect + i);
	    //	    hexdump_sect (log, (unsigned char *)buf + i * 512);
	  }
	  fflush (log);
	}

	return err;
}
