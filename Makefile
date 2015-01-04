tst : tst.c libcut.h
	$(CC) -std=c11 $< -o $@
