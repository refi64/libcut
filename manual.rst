The libcut manual
=================

Test cases
**********

Libcut defines the ``LIBCUT_TEST`` macro to create test cases:

.. code-block:: c
   
   #include <libcut.h>
   
   LIBCUT_TEST(test_name) {
       ...
   }

.. warning:: Test case names are function names. That means the above example creates a function named test_name that holds the test. You should there pick unique test case names; I usually just put a ``test_`` prefix on the name.

Main function
*************

You can use ``LIBCUT_MAIN`` to define an automatic main function:

.. code-block:: c
   
   #include <libcut.h>
   
   LIBCUT_TEST(test_name) { ... }
   
   LIBCUT_MAIN(test_name)

``LIBCUT_MAIN`` takes a list of defined test cases and runs them.

Comparisons
***********

The ``LIBCUT_TEST_*`` macros are the actual comparison macros:

.. code-block:: c
   
   #include <libcut.h>
   
   LIBCUT_TEST(test_x) {
       // test equal
       LIBCUT_TEST_EQ(1, 1);
       // test not equal
       LIBCUT_TEST_NE(1, 0);
       // test less-than, greater-than, less-than-or-equal-to, and greater-than-or-equal-to, respectively
       LIBCUT_TEST_LT(1, 2);
       LIBCUT_TEST_GT(2, 1);
       LIBCUT_TEST_LE(1, 1);
       LIBCUT_TEST_GE(1, 1);
       // for strings, use the LIBCUT_TEST_STR* macros
       // string equality
       LIBCUT_TEST_STREQ("a", "a");
       // string non-equality
       LIBCUT_TEST_STRNE("a", "b");
   }
   
   LIBCUT_MAIN(test_x)

Custom comparisons
******************

Use ``LIBCUT_TEST_CMP`` to create custom test cases:

.. code-block:: c
   
   // format:
   // LIBCUT_TEST_CMP(left-hand-side, right-hand-side, the test expression, what to print if it goes wrong)
   // example (this is how LIBCUT_TEST_EQ is defined):
   LIBCUT_TEST_CMP(1, 1, 1 == 1, "!=")
   // same as LIBCUT_TEST_EQ(1, 1)

Memory tracking
***************

Libcut can track memory you allocate and free it when the current test is over. Simply use ``LIBCUT_TRACK``:

.. code-block:: c
   
   void* m = malloc(5);
   LIBCUT_TRACK(m);
   // once the current test ends, *m* will be freed

If you want to untrack a memory block that you already tracked, use ``LIBCUT_UNTRACK``:

.. code-block:: c
   
   void* m = malloc(5);
   LIBCUT_TRACK(m);
   LIBCUT_UNTRACK(m);
   // *m* will not be freed because it was untracked
