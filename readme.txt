
2002.10.12

IT'S ALIVE!

The system seems to work.  That is, it does basic multi-tasking without
locking up.

It is now based on the GCC compiler and linker.  The memory.x file works
(quite well).  The only thing missing is a decent CRT0 file.  The one I
have does very little... in fact, it does not initialize "initialized
data".  So if you turn the board off, you'll need to reload the program
in order for the idata to be re-initialized.

The main problems that were solved this week: (1) I push three more
values onto the stack... those are the three pseudo-registers that GCC
invents and stores in the zero page.  (2) I re-did the initialization
in premain() so that it sets the three time-critical registers before
the 64th clock cycle.

Again, I would also like to take a look at the stuff I saw earlier in
locks.h.  I am not entirely happy with my INTR_ON/OFF macros.

Also, play with timing.  What's the deal with ECLOCK?


---


2002.09.29

New today... using GCC compiler (14MB!) from www.gnu-m68hc11.org.

Compiles OK, for the most part.  Still some small things left out.  The
assembler does not like the CCR macros to turn on and off interupts within the
control code register.

Would be nice to steal the exit() function and the stuff in locks.h.

Does not link properly.  Learn to make a good memory map in linker.x file.
Then firgure out how to position interrupt vectors in the proper place (both
special and normal ones).

