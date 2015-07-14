tst : tst.c libcut.h
	$(CC) $(CFLAGS) -g -std=c11 $< -o $@
