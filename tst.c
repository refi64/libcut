#include "libcut.h"

LIBCUT_TEST(xyz) {
    LIBCUT_TEST_EQ(1, 1);
    LIBCUT_TEST_STREQ("a", "ab");
    LIBCUT_TEST_CMP(1, 0, 1 == 0, "!=");
}

LIBCUT_TEST(abc) {
    LIBCUT_TEST_EQ(1, 0);
}

LIBCUT_MAIN(abc, xyz)
