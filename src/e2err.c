/***************************************************************************
 * error code routine
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "e2err.h"


struct errlist {
	int value;
	char *text;
} errors[] = {
	ENOENT,		"No such file or directory",
	ENOTDIR,	"Not a directory",
	0x07,		"Illegal disk number",
	0x80,		"Disk not ready",
	0xaa,		"Disk not ready",
	E2E_NOCWD,	"E2CWD environment not specified",
	E2E_BADCWD,	"E2CWD environment has bad format",
	E2E_BADFS,	"Disk does not contain an ext2 file system",
	E2E_BADPART,"Disk partitioning is corrupted",
	E2E_BADMEM,	"Not enough memory",
};

#define ERRCOUNT (sizeof(errors)/sizeof(errors[0]))



void
e2_err(char * text, int err)
{
	int i;

	for (i=0; i<ERRCOUNT; i++) {
		if (errors[i].value == err) {
			fprintf(stderr,"%s: %s\n",text,errors[i].text);
			exit(1);
		}
	}

	fprintf(stderr,"%s: Error number %d\n",text,err);
	exit(1);
}
