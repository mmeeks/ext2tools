/***************************************************************************
 * ldisk.h - Routines for handling logical disks
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/
 
#include <sys/types.h>

extern ino_t cwdino;			/* Current directory inode number */

extern int nametodisk(int *disk, long *offset, long *length);


