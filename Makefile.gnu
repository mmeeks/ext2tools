VERSION=0.0.1

all:
	echo "foo"

dist:
	git tag v$(VERSION)
	git archive --format=tar --prefix=ext2tool-$(VERSION)/ v$(VERSION) | bzip2  > ext2tool-$(VERSION).tar.bz2
