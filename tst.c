#include "libcut.h"
#include <stdio.h>

char s[] = "123";

LIBCUT_TEST(xyz) {
    void* m = malloc(10);
    LIBCUT_TRACK(m);
    LIBCUT_TRACK(s);
    LIBCUT_UNTRACK(s);
    LIBCUT_TEST_EQ(1, 1);
    LIBCUT_TEST_STREQ("a", "ab");
    LIBCUT_TEST_CMP(1, 0, 1 == 0, "!=");
}

LIBCUT_TEST(abc) {
    LIBCUT_TEST_EQ(1, 0);
}

LIBCUT_TEST(ptr) {
    char* p = "123";
    LIBCUT_TEST_EQ((void*)p, NULL);
}

LIBCUT_MAIN(abc, xyz, ptr)
