all:	lib util

lib:
	$(MAKE) -C ext2fs

util:
	$(MAKE) -C src


clean:	lclean uclean

lclean:
	$(MAKE) -C ext2fs clean

uclean:
	$(MAKE) -C src clean
