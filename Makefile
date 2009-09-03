VERSION=0.0.2
AR=i386

all:
	$(MAKE) -C ext2fs
	$(MAKE) -C src

clean:
	$(MAKE) -C ext2fs clean
	$(MAKE) -C src clean

dist:
	git tag -f v$(VERSION)
	git archive --format=tar --prefix=ext2tools-$(VERSION)/ v$(VERSION) | bzip2  > ext2tools-$(VERSION).tar.bz2
