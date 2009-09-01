VERSION=0.1


dist:
        git archive --format=tar --prefix=ext2tools-$(VERSION)/ v$(VERSION) | \
                bzip2  > ext2tools-$(VERSION).tar.bz2
