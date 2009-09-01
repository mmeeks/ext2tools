/***************************************************************************
 * diskio.h - Basic disk I/O routines
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/
 
extern int readdisk(int disk, int start, int nsect, void *buf);
