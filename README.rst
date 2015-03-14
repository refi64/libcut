Libcut
======

Libcut (**Lib** rary for **C** **U** nit **T** esting) is an easy-to-use unit testing framework designed for C11. It's in early development, but it's very usable, consisting of a single header file (*libcut.h*). See `the manual <https://github.com/kirbyfan64/libcut/blob/master/manual.rst>`_

The output is nice and colorful, too; here's the output from running the *tst.c* program included::
   
   FF
   error in test abc at tst.c:10: 1 != 0
   error in test xyz at tst.c:5: 'a' != 'ab'
   Tests run: 2
   Tests succeeded: 0
   Tests failed: 2
