/* RCODE.H - Parity chk and gen matrix storage, and procedures to read them.*/

/* Copyright (c) 2000 by Radford M. Neal 
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


/* VARIABLES HOLDING DATA READ.  These are declared for real in rcode.c. */

extern mod2sparse *H;	/* Parity check matrix */

extern int M;		/* Number of rows in parity check matrix */
extern int N;		/* Number of columns in parity check matrix */



/* PROCEDURES FOR READING DATA. */

void read_pchk (char *);

