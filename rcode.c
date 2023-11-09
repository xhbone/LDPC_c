/* RCODE.C - Procedures to read parity check and generator matrices. */

/* Copyright (c) 2000, 2001 by Radford M. Neal 
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "intio.h"
#include "open.h"
#include "mod2sparse.h"
#include "rcode.h"


/* VARIABLES DECLARED IN RCODE.H.  These global variables are set to
   representations of the parity check and generator matrices by read_pchk
   and read_gen. */

mod2sparse *H;		/* Parity check matrix */

int M;			/* Number of rows in parity check matrix */
int N;			/* Number of columns in parity check matrix */




/* READ PARITY CHECK MATRIX.  Sets the H, M, and N global variables.  If an
   error is encountered, a message is displayed on standard error, and the
   program is terminated. */

void read_pchk
( char *pchk_file
)
{
  FILE *f;

  f = open_file_std(pchk_file,"rb");
  if (f==NULL)
  { fprintf(stderr,"Can't open parity check file: %s\n",pchk_file);
    exit(1);
  }

  if (intio_read(f)!=('P'<<8)+0x80)
  { fprintf(stderr,"File %s doesn't contain a parity check matrix\n",pchk_file);
    exit(1);
  }

  H = mod2sparse_read(f);

  if (H==0)
  { fprintf(stderr,"Error reading parity check matrix from %s\n",pchk_file);
    exit(1);
  }

  M = mod2sparse_rows(H);
  N = mod2sparse_cols(H);

  fclose(f);
}


