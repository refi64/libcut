.PHONY : install

INSTALL=install -c
PREFIX=/usr/local
LN=ln

tst : tst.c libcut.h
	$(CC) $(CFLAGS) -g -std=c11 $< -o $@

install :
	$(INSTALL) libcut.h $(PREFIX)/include/libcut.h

link :
	$(LN) -sf $$PWD/libcut.h $(PREFIX)/include/libcut.h
