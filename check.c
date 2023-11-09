/* CHECK.C - Compute parity checks and other stats on decodings. */

/* Copyright (c) 2001 by Radford M. Neal 
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
#include <math.h>

#include "mod2sparse.h"
#include "check.h"



/* COMPUTE PARITY CHECKS.  Returns the number of parity checks violated by
   dblk.  The results of all the parity checks are stored in pchk. */

int check
( mod2sparse *H,	/* Parity check matrix */
  char *dblk,		/* Guess for codeword */
  char *pchk		/* Place to store parity checks */
)
{
  int M, i, c;
  int cnt=0;

  FILE *fp;
  // fp=fopen("chk.dat","a");
  M = mod2sparse_rows(H);

  mod2sparse_mulvec (H, dblk, pchk);

 
  c = 0;
  for (i = 0; i<M; i++) 
  { c += pchk[i];
  	// if(pchk[i]==1) fprintf(fp,"%d ",i);
  	if(pchk[i]==0) cnt++;    
  }
  // fprintf(fp,"%d ",cnt);
  // printf("%d\n",cnt);
  // fprintf(fp,"\n");
  return c;
}

