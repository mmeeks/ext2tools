/***************************************************************************
 * msdos_io.c --- This is the MS-DOS I/O interface to the I/O manager.
 *
 * Copyright (C) 1995 Claus Tondering, ct@login.dknet.dk
 *
 * Based on unix_io.c, which is copyright (C) 1993 Theodore Ts'o.
 *
 * This file may be redistributed under the terms of the GNU Public License.
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>

#include "ext2fs/com_err.h"
#include "ext2fs/ext2_err.h"
#include "ext2fs/io.h"
#include "diskio.h"
#include "ldisk.h"
/*
 * For checking structure magic numbers...
 */

#define EXT2_CHECK_MAGIC(struct, code) \
	  if ((struct)->magic != (code)) return (code)
  
struct msdos_private_data {
	long	magic;
	int		diskno;
	long	offset;
	long	length;
	int		flags;
	char	*buf;
	int	buf_block_nr;
};

static errcode_t msdos_open(const char *name, int flags, io_channel *channel);
static errcode_t msdos_close(io_channel channel);
static errcode_t msdos_set_blksize(io_channel channel, int blksize);
static errcode_t msdos_read_blk(io_channel channel, unsigned long block,
			       int count, void *data);
static errcode_t msdos_write_blk(io_channel channel, unsigned long block,
				int count, const void *data);
static errcode_t msdos_flush(io_channel channel);

static struct struct_io_manager struct_msdos_manager = {
	EXT2_ET_MAGIC_IO_MANAGER,
	"MS-DOS I/O Manager",
	msdos_open,
	msdos_close,
	msdos_set_blksize,
	msdos_read_blk,
	msdos_write_blk,
	msdos_flush
};

io_manager msdos_io_manager = &struct_msdos_manager;

static errcode_t msdos_open(const char *name, int flags, io_channel *channel)
{
	io_channel	io = NULL;
	struct msdos_private_data *data = NULL;
	errcode_t	retval;

	io = (io_channel) malloc(sizeof(struct struct_io_channel));
	if (!io)
		return ENOMEM;
	io->magic = EXT2_ET_MAGIC_IO_CHANNEL;
	data = (struct msdos_private_data *)
		malloc(sizeof(struct msdos_private_data));
	if (!data) {
		retval = ENOMEM;
		goto cleanup;
	}
	io->manager = msdos_io_manager;
	io->private_data = data;
	io->block_size = 1024;

	memset(data, 0, sizeof(struct msdos_private_data));
	data->magic = EXT2_ET_MAGIC_MSDOS_IO_CHANNEL;
	data->buf = malloc(io->block_size);
	data->buf_block_nr = -1;
	if (!data->buf) {
		retval = ENOMEM;
		goto cleanup;
	}
	
	retval=nametodisk(&data->diskno, &data->offset, &data->length);
	if (retval)
		goto cleanup;

	if (flags&IO_FLAG_RW) {
		retval = EACCES;
		goto cleanup;
	}
	
	*channel = io;
	return 0;

 cleanup:
	if (io)
		free(io);
	if (data) {
		if (data->buf)
			free(data->buf);
		free(data);
	}
	return retval;
}

static errcode_t msdos_close(io_channel channel)
{
	struct msdos_private_data *data;
	errcode_t	retval = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct msdos_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_MSDOS_IO_CHANNEL);
	
	if (data->buf)
		free(data->buf);
	if (channel->private_data)
		free(channel->private_data);
	if (channel->name)
		free(channel->name);
	free(channel);
	return retval;
}

static errcode_t msdos_set_blksize(io_channel channel, int blksize)
{
	struct msdos_private_data *data;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct msdos_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_MSDOS_IO_CHANNEL);

	if (channel->block_size != blksize) {
		channel->block_size = blksize;
		free(data->buf);
		data->buf = malloc(blksize);
		if (!data->buf)
			return ENOMEM;
		data->buf_block_nr = -1;
	}
	return 0;
}


static errcode_t msdos_read_blk(io_channel channel, unsigned long block,
			       int count, void *buf)
{
	struct msdos_private_data *data;
	errcode_t	retval;
	size_t		size;
	ext2_loff_t	location;
	int		actual = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	data = (struct msdos_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(data, EXT2_ET_MAGIC_MSDOS_IO_CHANNEL);

	/*
	 * If it's in the cache, use it!
	 */
	if ((count == 1) && (block == data->buf_block_nr)) {
		memcpy(buf, data->buf, channel->block_size);
		return 0;
	}
	size = (count < 0) ? -count : count * channel->block_size;
	location = (ext2_loff_t) block * channel->block_size;

	retval = readdisk(data->diskno, data->offset + location/512, size/512, buf);
	if (retval)
		goto error_out;

	if (count == 1) {
		data->buf_block_nr = block;
		memcpy(data->buf, buf, size);	/* Update the cache */
	}
	return 0;
	
error_out:
	memset((char *) buf+actual, 0, size-actual);
	if (channel->read_error)
		retval = (channel->read_error)(channel, block, count, buf,
					       size, actual, retval);
	return retval;
}

static errcode_t msdos_write_blk(io_channel channel, unsigned long block,
				int count, const void *buf)
{
	return EACCES;
}

/*
 * Flush data buffers to disk.
 * This is a no-op.
 */
static errcode_t msdos_flush(io_channel channel)
{
	return 0;
}

