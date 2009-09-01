VERSION=0.0.1

all:
	echo "foo"

dist:
	git tag v$(VERSION)
	git archive --format=tar --prefix=ext2tools-$(VERSION)/ v$(VERSION) | bzip2  > ext2tools-$(VERSION).tar.bz2
