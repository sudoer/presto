
2002.09.29

New today... using GCC compiler (14MB!) from www.gnu-m68hc11.org.

Compiles OK, for the most part.  Still some small things left out.  The
assembler does not like the CCR macros to turn on and off interupts within the
control code register.

Would be nice to steal the exit() function and the stuff in locks.h.

Does not link properly.  Learn to make a good memory map in linker.x file.
Then firgure out how to position interrupt vectors in the proper place (both
special and normal ones).

