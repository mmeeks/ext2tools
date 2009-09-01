/***************************************************************************
 * error codes for ext2 tools
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/


#define E2E_NOCWD	2001		/* E2CWD environment not specified */
#define E2E_BADCWD	2002		/* E2CWD environment has bad format */
#define E2E_BADFS	2003		/* Disk does not contain an ext2 file system */
#define E2E_BADPART	2004		/* Disk partitioning is corrupted */
#define E2E_BADMEM	2005		/* Not enough memory */

extern void e2_err(char *,int);
