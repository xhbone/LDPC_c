/* DEC.H - Interface to decoding procedures. */

/* Copyright (c) 2018/07/13 by Radford M. Neal 
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


/* DECODING METHOD, ITS PARAMETERS, AND OTHER VARIABLES.  The global variables 
   declared here are located in dec.c. */

//ms lratio=ln p0/p1 lratio=-lratio 
//ms2 lratio=ln p1/p0  lratio=lratio


typedef enum {MS_layered_awgn,MS_flooding_awgn,MS_flooding_awgn_orig,OMS_ET,OMS_layered_awgn,OMS_flooding_awgn,BP_layered_awgn,BP_flooding_awgn,AdjMS_layered_awgn,AdjMS_flooding_awgn,stochastic_BP_awgn} decoding_method;

extern decoding_method dec_method; /* Decoding method to use */

unsigned MS_layered_awgn_dec(mod2sparse *, double *, char *, char *, int);
void initms_layered_awgn(mod2sparse *, double *, char *);
void iterms_layered_awgn(mod2sparse *, double *, char *);

unsigned MS_flooding_awgn_dec(mod2sparse *, double *, char *, char *, int);
void initms_flooding_awgn(mod2sparse *, double *, char *);
void iterms_flooding_awgn(mod2sparse *, double *, char *,int);

unsigned MS_flooding_awgn_dec_orig(mod2sparse *, double *, char *, char *, int);
void initms_flooding_awgn_orig(mod2sparse *, double *, char *);
void iterms_flooding_awgn_orig(mod2sparse *, double *, char *);
// void iterms_flooding_awgn(mod2sparse *, double *, char *);

unsigned OmsET_decode (mod2sparse *, double *, char *, char *, int);
void init_OmsET(mod2sparse *, double *, char *);
int iter_OmsET(mod2sparse *, double *, char *, double);

unsigned OMS_layered_awgn_dec(mod2sparse *, double *, char *, char *, int);
void initoms_layered_awgn(mod2sparse *, double *, char *);
void iteroms_layered_awgn(mod2sparse *, double *, char *);

unsigned OMS_flooding_awgn_dec(mod2sparse *, double *, char *, char *, int);
void initoms_flooding_awgn(mod2sparse *, double *, char *);
void iteroms_flooding_awgn(mod2sparse *, double *, char *);

unsigned BP_layered_awgn_dec(mod2sparse *, double *, char *, char *, int);
void initbp_layered_awgn(mod2sparse *, double *, char *);
void iterbp_layered_awgn(mod2sparse *, double *, char *);

unsigned BP_flooding_awgn_dec(mod2sparse *, double *, char *, char *, int);
void initbp_flooding_awgn(mod2sparse *, double *, char *);
void iterbp_flooding_awgn(mod2sparse *, double *, char *);

unsigned AdjMS_layered_awgn_dec(mod2sparse *, double *, char *, char *, int);
void initadjms_layered_awgn(mod2sparse *, double *, char *);
void iteradjms_layered_awgn(mod2sparse *, double *, char *);

unsigned AdjMS_flooding_awgn_dec(mod2sparse *, double *, char *, char *, int);
void initadjms_flooding_awgn(mod2sparse *, double *, char *);
void iteradjms_flooding_awgn(mod2sparse *, double *, char *);


typedef struct subnode
{
   int in1,in2;
   int out;
   double beta;
   double *P;
}subnode;

void init_subnode(subnode *);
void calculate_subnode(subnode *);

typedef struct stage
{
   int in_num,out_num;
   int *in;
   int *out;
   subnode *sub_node;
}stage;

void generate_stage(stage *substage,int in_num);

typedef struct dual_tree
{
   int in_num,out_num,S;
   int *in;
   int *out;
   stage *substage;
}dual_tree;

void generate_dual_tree(dual_tree *tree,int dv);
void compute_dualtree(dual_tree *tree);
void sort_inputs_subnode_1(stage *substage);
void sort_inputs_subnode_2(stage *substage,int s,int S);
void sort_inputs_subnode_3(stage *substagggge,int s,int S);


// void del_dualtree(dual_tree *tree);
// void del_stage(stage *substage);








typedef struct node1
{
   int in;
   int chn;
   int out[2];
}node1;

typedef struct node2
{
   int in[2];
   int chn;
   int out[2];
   double *P;
}node2;

typedef struct node3
{
   int in[3];
   int chn;
   int out[3];
   double *P;
}node3;

typedef struct node4
{
   int in[4];
   int chn;
   int out[4];
   double *P;
}node4;

typedef struct node
{
   int n1_num,n2_num,n3_num,n4_num;
   node1 *n1;
   node2 *n2;
   node3 *n3;
   node4 *n4;
}node;

node *init_node(int *dv,int N);
void compute_vn_node(node *vn_node,int seed2,int seed3,int seed4);
void del_node(node *vn_node);
void post_hard_decision(mod2sparse *H,char *dblk,char *pchk,int *counter);
double rng(int *sequence, int N);
void dec2vec(int a, int *x, int pow);

unsigned stochastic_BP_flooding_awgn(mod2sparse *,double *,char *,char *,int );
int init_stochastic_bp_flooding_awgn(mod2sparse *,double *,int* ,int * );
int F_proc_stochastic(int in1,int in2);
void G_TFM(int in1,int in2,int *result,double *P,double beta,double r);
void G_TFM2(int in1,int in2,int *result,double *P,double beta,int seed);
void iter_stochastic_bp_flooding_awgn(mod2sparse *H,double *lratio,char *dblk,int *counter,node *vn_node,int *seed);


