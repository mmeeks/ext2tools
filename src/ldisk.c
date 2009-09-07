/***************************************************************************
 * ldisk.c - Routines for handling logical disks
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/
 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ext2_fs.h>

#include "e2err.h"
#include "part.h"
#include "diskio.h"
#include "ldisk.h"


ino_t cwdino;

/****************************************************************************
 * nametodisk() converts a disk identification in the E2CWD environment to a
 * BIOS disk number and an offset and a length (both in blocks of 512 bytes).
 *
 * Returns an error code
 ****************************************************************************/

int
nametodisk(int *disk, long *offset, long *length)
{
	int err, fstype, subdisk;
	struct part_desc pl[16];
	char env[80], *envp, *pos1, *pos2, *cp;

	/* Decode E2CWD environment */
	envp=getenv("E2CWD");
	
	if (!envp)
		return E2E_NOCWD;

	strcpy(env,envp);			/* We are going to modify it, so we make a copy */

	pos1=strchr(env,':');

	if (!pos1)
		return E2E_BADCWD;

	pos2=strchr(pos1+1,':');

	*pos1 = 0;
	pos1++;

	for (cp=env; *cp; cp++)
		if (!isdigit(*cp))
			return E2E_BADCWD;

	*disk = atoi(env);

	if (pos2) {
		*pos2 = 0;
		pos2++;
	}

	for (cp=pos1; *cp; cp++)
		if (!isdigit(*cp))
			return E2E_BADCWD;

	subdisk = atoi(pos1)-1;

	if (pos2) {
		for (cp=pos2; *cp; cp++)
			if (!isdigit(*cp))
				return E2E_BADCWD;

		cwdino = atoi(pos2);
	}
	else
		cwdino = EXT2_ROOT_INO;


	/* Check that disk contains an ext2 file system */

	if (!(*disk & 0x80)) {
		/* Floppy disk */
		*offset = 0;
		*length = 1440*2; /* TBD */
		fstype=getfstype(*disk, 0, 0);

		if (fstype<0)
			return -fstype;

		if (fstype!=EXT2FS)
			return E2E_BADFS;
		
		return 0;
	}
	
	err=get_part(*disk, pl);
	if (err)
		return err;

	if (pl[subdisk].is_extended)
		return E2E_BADFS;

	fstype=getfstype(*disk, pl[subdisk].parttype, pl[subdisk].start);
	if (fstype<0)
		return -fstype;
	if (fstype!=EXT2FS)
		return E2E_BADFS;

	*offset = pl[subdisk].start;
	*length = pl[subdisk].length;

	return 0;
}
