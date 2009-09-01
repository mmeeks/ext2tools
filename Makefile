VERSION=0.0.2

all:
	$(MAKE) -C ext2fs
	$(MAKE) -C src

clean:
	$(MAKE) -C ext2fs
	$(MAKE) -C src

dist:
	git tag -f v$(VERSION)
	git archive --format=tar --prefix=ext2tools-$(VERSION)/ v$(VERSION) | bzip2  > ext2tools-$(VERSION).tar.bz2
