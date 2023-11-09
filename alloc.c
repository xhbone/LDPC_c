/* ALLOC.C - Routine to allocate memory and complain if it doesn't work. */

/* Copyright (c) 1996 by Radford M. Neal 
 *
 * Permission is granted for anyone to copy, use, or modify this program 
 * for purposes of research or education, provided this copyright notice 
 * is retained, and note is made of any changes that have been made. 
 *
 * This program is distributed without any warranty, express or implied.
 * As this program was written for research purposes only, it has not been
 * tested to the degree that would be advisable in any important application.
 * All use of this program is entirely at the user's own risk.
 */

#include <stdlib.h>
#include <stdio.h>

#include "alloc.h"


/* ALLOCATE SPACE AND CHECK FOR ERROR.  Calls 'calloc' to allocate space,
   and then displays an error message and exits if the space couldn't be
   found. */

void *chk_alloc
( unsigned n,		/* Number of elements */
  unsigned size		/* Size of each element */
)
{ 
  void *p;

  p = calloc(n,size);

  if (p==0)
  { fprintf(stderr,"Ran out of memory (while trying to allocate %d bytes)\n",
      n*size);
    exit(1);
  }

  return p;
}
