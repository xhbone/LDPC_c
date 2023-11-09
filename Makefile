# MAKEFILE FOR LDPC PROGRAMS & ASSOCIATED UTILITIES.

# Copyright (c) 2000, 2001 by Radford M. Neal 
#
# Permission is granted for anyone to copy, use, or modify this program 
# for purposes of research or education, provided this copyright notice 
# is retained, and note is made of any changes that have been made. 
#
# This program is distributed without any warranty, express or implied.
# As this program was written for research purposes only, it has not been
# tested to the degree that would be advisable in any important application.
# All use of this program is entirely at the user's own risk.


# NOTE:  The natural random numbers in "randfile" are accessed by the
# 'rand' module via a path to this directory.  Change the definition of
# RAND_FILE in the compilation command for rand.c below if this is not
# appropriate.

# NOTE:  This makefile is trivial, simply recompiling everything from
# scratch every time.  Since this takes only about 5 seconds on a modern
# PC, there's no point in putting in dependency-based rules, which just 
# make things more complex and error-prone.


COMPILE = gcc -c -O  # Command to compile a module from .c to .o
LINK =    gcc        # Command to link a program


# MAKE ALL THE MAIN PROGRAMS.  First makes the modules used.

progs:	modules
	$(COMPILE) main.c
	$(LINK) main.o rcode.o dec.o quantization.o alloc.o intio.o check.o open.o mod2sparse.o rand.o -lm -o main 
	
	$(COMPILE) make-pchk.c
	$(LINK) make-pchk.o mod2sparse.o rcode.o alloc.o intio.o open.o -lm -o make-pchk
	rm -rf *.o
# MAKE THE MODULES USED BY THE PROGRAMS.

modules:
	$(COMPILE) rcode.c
	$(COMPILE) dec.c
	$(COMPILE) quantization.c
	$(COMPILE) alloc.c
	$(COMPILE) intio.c
	$(COMPILE) check.c
	$(COMPILE) open.c
	$(COMPILE) mod2sparse.c
	$(COMPILE) -DRAND_FILE=\"./randfile\" rand.c


# CLEAN UP ALL PROGRAMS AND REMOVE ALL FILES PRODUCED BY TESTS AND EXAMPLES.


	
