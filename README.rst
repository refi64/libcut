Libcut
======

Libcut (**Lib** rary for **C** **U** nit **T** esting) is an easy-to-use unit testing framework designed for C11. It's in early development, but it's very usable, consisting of a single header file (*libcut.h*). Here's an example:

.. code-block:: c
   
   #include <libcut.h>
   
   // LIBCUT_TEST defines a new test
   LIBCUT_TEST(test_stuff) {
       LIBCUT_TEST_EQ(1, 1);
       LIBCUT_TEST_NE(1, 0);
       
       LIBCUT_TEST_STREQ("a", "a");
       LIBCUT_TEST_STRNE("a", "b");
       
       // LIBCUT_TEST_CMP lets you create custom comparisons.
       // It's defined like so:
       // LIBCUT_TEST_CMP(lhs, rhs, expr, err)
       // lhs  is the left-hand-side
       // rhs  is the right-hand-side
       //      both must be either a primitive (such as int) or a string
       //      strings are automatically single-quoted
       // expr is the condition
       // err  is the binary operator used when the test fails
       //      it's the opposite of the normal operator
       LIBCUT_TEST_CMP(1, 1, 1==1, "!=");
   }
   
   LIBCUT_MAIN(test_stuff) // define the main function

The output is nice and colorful, too; here's the output from running the *tst.c* program included::
   
   FF
   error in test abc at tst.c:10: 1 != 0
   error in test xyz at tst.c:5: 'a' != 'ab'
   Tests run: 2
   Tests succeeded: 0
   Tests failed: 2
