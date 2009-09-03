/***************************************************************************
 * diskio.h - Basic disk I/O routines
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/

typedef unsigned char u_char; 
typedef unsigned short u_short; 
typedef unsigned long u_long; 

extern int readdisk(int disk, int start, int nsect, void *buf);
