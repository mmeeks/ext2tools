VERSION=1.40
AR=i386

all:
	$(MAKE) -C e2p
	$(MAKE) -C et
	$(MAKE) -C ext2fs
	$(MAKE) -C src

clean:
	$(MAKE) -C e2p clean
	$(MAKE) -C et clean
	$(MAKE) -C ext2fs clean
	$(MAKE) -C src clean

dist:
	git tag -f v$(VERSION)
	git archive --format=tar --prefix=ext2tools-$(VERSION)/ v$(VERSION) | bzip2  > ext2tools-$(VERSION).tar.bz2
