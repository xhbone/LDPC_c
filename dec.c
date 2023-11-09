#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <limits.h>

#include "mod2sparse.h"
#include "check.h"
#include "dec.h"
#include "rand.h"
#include "quantization.h"

decoding_method dec_method;

// macros for CH, V2C
#define TOTAL_PART (7 - 1)     // "1" represents the sign bit
#define FRAC_PART (2)
#define DENOMINATOR (1 << FRAC_PART)
#define UP_BOUND (((1 << TOTAL_PART) - 1) * (1.0 / DENOMINATOR))
#define LOW_BOUND (-UP_BOUND)

// macros for C2V, extMes, appMes
#define TOTAL_PART_WID_INCR (TOTAL_PART + 2)
#define FRAC_PART_WID_INCR (FRAC_PART + 2)
#define DENOMINATOR_WID_INCR (1 << FRAC_PART_WID_INCR)
#define UP_BOUND_WID_INCR (((1 << TOTAL_PART_WID_INCR) - 1) * (1.0 / DENOMINATOR_WID_INCR))
#define LOW_BOUND_WID_INCR (-UP_BOUND_WID_INCR)


double Quantization(double x)
{
        if (x >= UP_BOUND) {
            return UP_BOUND;
		} else if (x <= LOW_BOUND) {
            return LOW_BOUND;
		} else {
            return ((int)(x * DENOMINATOR) * 1.0) / DENOMINATOR;
		}
}

double QuantizationWidIncr(double x)
{
        if (x >= UP_BOUND_WID_INCR) {
            return UP_BOUND_WID_INCR;
		} else if (x <= LOW_BOUND_WID_INCR) {
            return LOW_BOUND_WID_INCR;
		} else {
            return ((int)(x * DENOMINATOR_WID_INCR) * 1.0) / DENOMINATOR_WID_INCR;
		}
}

double quantization1(double x, double up, double low)
{
        if(x>=up)
                return up;
        else if(x<=low)
                return low;
        else 
                return ((int)(x*2)*1.0)/2; 
}
double quantization2(double x, double up, double low)
{
        if(x>=up)
                return up;
        else if(x<=low)
                return low;
        else 
                return ((int)(x*4)*1.0)/4; 
}
double quantization3(double x, double up, double low)
{
        if(x>=up)
                return up;
        else if(x<=low)
                return low;
        else 
                return ((int)(x*8)*1.0)/8; 
}
double quantization4(double x, double up, double low)
{
        if(x>=up)
                return up;
        else if(x<=low)
                return low;
        else 
                return ((int)(x*16)*1.0)/16; 
}
//unmodified
/***********  MS_layered_awgn ****************/
unsigned MS_layered_awgn_dec(mod2sparse *H, double *lratio, char *dblk, char *pchk, int max_iter)
{
	int N,M, n, c;
	int j,i = 0;
	M = mod2sparse_rows(H);
	N = mod2sparse_cols(H);
	initms_layered_awgn(H,lratio,dblk);
	for (n = 0; ; n++)
	{
		c = check(H,dblk,pchk);
		if (n==max_iter || n==-max_iter || (max_iter>0 && c==0))
		{
			break;
		}
		iterms_layered_awgn(H,lratio,dblk);
	}
	return n;
}
void initms_layered_awgn(mod2sparse *H, double *lratio, char *dblk)
{
	mod2entry *e;
	int N,j;
	N = mod2sparse_cols(H);
	for (j = 0; j<N; j++)
	{
		for(e = mod2sparse_first_in_col(H,j); !mod2sparse_at_end(e); e = mod2sparse_next_in_col(e))
		{
			e->pr=lratio[j];
			e->lr=0;
		}
		dblk[j] = (lratio[j]<0);
	}
}
void iterms_layered_awgn(mod2sparse *H, double *lratio, char *dblk)
{
	double dl, t,l,m1,m2;
	mod2entry *e;
	int N,M, i, j,k,sign;
	M = mod2sparse_rows(H);
	N = mod2sparse_cols(H);
	for (i = 0; i < M; i++)
	{
		k=0;
		m1=1000;
		m2=1000;
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			e->pr = lratio[mod2sparse_col(e)] - e->lr;
			dl=m1;
			m1=(dl>fabs(e->pr))?fabs(e->pr):dl;
			m2=(dl>fabs(e->pr))?dl:(m2>fabs(e->pr))?fabs(e->pr):m2;
			l=(e->pr<0)?1:0;
			k=k+l;
		}  	
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			t = (fabs(e->pr)==m1)?m2:m1;
			l= (e->pr<0)?1:0;
			sign=((k%2)==l)?1:-1;
			e->lr = sign*t;
		}
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{ 
			j = mod2sparse_col(e);
			lratio[j] = e->pr + e->lr;
		}
	}
	for(j=0;j<N;j++)
	{
		dblk[j] = lratio[j]<0;
	}
}



//unmodified
/***********  NMS_flooding_awgn ****************/

unsigned MS_flooding_awgn_dec_orig(mod2sparse *H, double *lratio, char *dblk, char *pchk, int max_iter)
{
	int N,M, n, c;
	int j,i = 0;
	mod2entry *e;
	M = mod2sparse_rows(H);
	N = mod2sparse_cols(H);
	initms_flooding_awgn_orig(H,lratio,dblk);
	for (n = 0; ; n++)
	{
		c = check(H,dblk,pchk);
		if (n==max_iter || n==-max_iter || (max_iter>0 && c==0))
		{
			break;
		}
		iterms_flooding_awgn_orig(H,lratio,dblk);
		/* if (n==10 || n==20){
			for (j = 0; j<N; j++)
			{
			for(e = mod2sparse_first_in_col(H,j); !mod2sparse_at_end(e); e = mod2sparse_next_in_col(e))
			{
				e->pr=1.5*e->pr;
			}
			
			}
			
		} */
	}
	return n;
}
void initms_flooding_awgn_orig(mod2sparse *H, double *lratio, char *dblk)
{
	mod2entry *e;
	int N,j;
	N = mod2sparse_cols(H);
	for (j = 0; j<N; j++)
	{
		for(e = mod2sparse_first_in_col(H,j); !mod2sparse_at_end(e); e = mod2sparse_next_in_col(e))
		{
			e->pr=lratio[j];
			e->lr=0;
		}
		dblk[j] = (lratio[j]<0);
	}
}
void iterms_flooding_awgn_orig(mod2sparse *H, double *lratio, char *dblk)
{
	double pr,dl, t,l,m1,m2;
	mod2entry *e;
	int N,M, i, j,k,sign,hh;
	M = mod2sparse_rows(H);
	N = mod2sparse_cols(H);
	// CN. 
	for (i = 0; i < M; i++)
	{
		k=0;
		m1=1000;
		m2=1000;
		hh=0;
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			dl=m1;
			m1=(dl>fabs(e->pr))?fabs(e->pr):dl;
			m2=(dl>fabs(e->pr))?dl:(m2>fabs(e->pr))?fabs(e->pr):m2;
			l=(e->pr<0)?1:0;
			k=k+l;
				/* if(hh%2==0){
				if(fabs(e->pr)<m1){
					m1=fabs(e->pr);
				}
				}
				if(hh%2==1){
				if(fabs(e->pr)<m2){
					m2=fabs(e->pr);
				}
				}
				hh=hh+1; */
			}
			
			/* dl =m1;
			m1 = m1<m2?m1:m2;
			m2 = dl>m2?dl:m2; */
			//printf("\n%lf %lf",m1,m2);
			
			
			/* m1=1000;
			m2=1000;
			for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
			{
				dl=m1;
				m1=(dl>fabs(e->pr))?fabs(e->pr):dl;
				m2=(dl>fabs(e->pr))?dl:(m2>fabs(e->pr))?fabs(e->pr):m2;
				l=(e->pr<0)?1:0;
				k=k+l;
			}
			printf("\n%lf %lf",m1,m2);
			getchar(); */
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			t = (fabs(e->pr)==m1)?m2:m1;
			l= (e->pr<0)?1:0;
			sign=((k%2)==l)?1:-1;
			e->lr = 0.75*sign*t;
		}
	}
	// VN.   
	for (j = 0; j<N; j++)
	{ 
		pr = lratio[j];
		for (e = mod2sparse_first_in_col(H,j);!mod2sparse_at_end(e);e = mod2sparse_next_in_col(e))
		{ 
			//e->pr = pr;
			pr+=e->lr;
		}
		dblk[j] = pr<0;
		//pr = 0;
		for (e = mod2sparse_last_in_col(H,j);!mod2sparse_at_end(e);e = mod2sparse_prev_in_col(e))
		{ 
			e->pr = pr - e->lr;     
			/* e->pr=t;
			pr += e->lr; */
		}
	}  
}


/***********  OMS_layered_awgn ****************/
unsigned OMS_layered_awgn_dec(mod2sparse *H, double *lratio, char *dblk, char *pchk, int max_iter)
{
	int N,M, n, c;
	int j,i = 0;
	M = mod2sparse_rows(H);
	N = mod2sparse_cols(H);
	initoms_layered_awgn(H,lratio,dblk);
	for (n = 0; ; n++)
	{
		c = check(H,dblk,pchk);
		if (n==max_iter || n==-max_iter || (max_iter>0 && c==0))
		{
			break;
		}
		iteroms_layered_awgn(H,lratio,dblk);
	}
	return n;
}
void initoms_layered_awgn(mod2sparse *H, double *lratio, char *dblk)
{
	mod2entry *e;
	int N,j;
	N = mod2sparse_cols(H);
	for (j = 0; j<N; j++)
	{
		for(e = mod2sparse_first_in_col(H,j); !mod2sparse_at_end(e); e = mod2sparse_next_in_col(e))
		{
			e->pr=lratio[j];
			e->lr=0;
		}
		dblk[j] = (lratio[j]<0);
	}
}
void iteroms_layered_awgn(mod2sparse *H, double *lratio, char *dblk)
{
	double dl, t,l,m1,m2;
	mod2entry *e;
	int N,M, i, j,k,sign;
	M = mod2sparse_rows(H);
	N = mod2sparse_cols(H);
	for (i = 0; i < M; i++)
	{
		k=0;
		m1=1000;
		m2=1000;
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			e->pr = lratio[mod2sparse_col(e)] - e->lr;
			dl=m1;
			m1=(dl>fabs(e->pr))?fabs(e->pr):dl;
			m2=(dl>fabs(e->pr))?dl:(m2>fabs(e->pr))?fabs(e->pr):m2;
			l=(e->pr<0)?1:0;
			k=k+l;
		}  	
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			t = (fabs(e->pr)==m1)?m2:m1;
			l= (e->pr<0)?1:0;
			sign=((k%2)==l)?1:-1;
			t = ((t-0.2)>=0)?t-0.2:0;
			e->lr = sign*t;
		}
 		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{ 
 			j = mod2sparse_col(e);
			lratio[j] = e->pr + e->lr;
		}
	}
	for(j=0;j<N;j++)
	{
		dblk[j] = lratio[j]<0;
	}
}



/***********  OMS_flooding_awgn ****************/
unsigned OMS_flooding_awgn_dec(mod2sparse *H, double *lratio, char *dblk, char *pchk, int max_iter)
{
	int N,M, n, c;
	int j,i = 0;
	M = mod2sparse_rows(H);
	N = mod2sparse_cols(H);
	initoms_flooding_awgn(H,lratio,dblk);
	for (n = 0; ; n++)
	{
		c = check(H,dblk,pchk);
		if (n==max_iter || n==-max_iter || (max_iter>0 && c==0))
		{
			break;
		}
		iteroms_flooding_awgn(H,lratio,dblk);
	}
	return n;
}
void initoms_flooding_awgn(mod2sparse *H, double *lratio, char *dblk)
{
	mod2entry *e;
	int N,j;
	N = mod2sparse_cols(H);
	for (j = 0; j<N; j++)
	{
		for(e = mod2sparse_first_in_col(H,j); !mod2sparse_at_end(e); e = mod2sparse_next_in_col(e))
		{
			e->pr=lratio[j];
			e->lr=0;
		}
		dblk[j] = (lratio[j]<0);
	}
}
void iteroms_flooding_awgn(mod2sparse *H, double *lratio, char *dblk)
{
	double pr,dl, t,l,m1,m2;
	mod2entry *e;
	int N,M, i, j,k,sign;
	M = mod2sparse_rows(H);
	N = mod2sparse_cols(H);
	/* CN. */
	for (i = 0; i < M; i++)
	{
		k=0;
		m1=1000;
		m2=1000;
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			dl=m1;
			m1=(dl>fabs(e->pr))?fabs(e->pr):dl;
			m2=(dl>fabs(e->pr))?dl:(m2>fabs(e->pr))?fabs(e->pr):m2;
			l=(e->pr<0)?1:0;
			k=k+l;
		}  	
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			t = (fabs(e->pr)==m1)?m2:m1;
			l= (e->pr<0)?1:0;
			sign=((k%2)==l)?1:-1;
			t = ((t-0.2)>=0)?t-0.2:0;
			e->lr = sign*t;
		}
	}
	/* VN. */  
	for (j = 0; j<N; j++)
	{ 
		pr = lratio[j];
		for (e = mod2sparse_first_in_col(H,j);!mod2sparse_at_end(e);e = mod2sparse_next_in_col(e))
		{ 
			e->pr = pr;
			pr+=e->lr;
		}
		dblk[j] = pr<0;
		pr = 0;
		for (e = mod2sparse_last_in_col(H,j);!mod2sparse_at_end(e);e = mod2sparse_prev_in_col(e))
		{ 
			t = e->pr + pr;     
			e->pr=t;
			pr += e->lr;
		}
	}  
}


/***********  BP_layered_awgn ****************/
unsigned BP_layered_awgn_dec(mod2sparse *H, double *lratio, char *dblk, char *pchk, int max_iter)
{
	int N,M, n, c;
	int j,i = 0;
	M = mod2sparse_rows(H);
	N = mod2sparse_cols(H);
	initbp_layered_awgn(H,lratio,dblk);
	for (n = 0; ; n++)
	{
 		c = check(H,dblk,pchk);
		if (n==max_iter || n==-max_iter || (max_iter>0 && c==0))
		{
			break;
		}
		iterbp_layered_awgn(H,lratio,dblk);
	}
	return n;
}
void initbp_layered_awgn(mod2sparse *H, double *lratio, char *dblk)
{
	mod2entry *e;
	int N,j,s;
	N = mod2sparse_cols(H);
	for (j = 0; j<N; j++)
	{
		for(e = mod2sparse_first_in_col(H,j); !mod2sparse_at_end(e); e = mod2sparse_next_in_col(e))
		{
			e->pr=lratio[j];
			e->lr=0;
		}
		dblk[j] = (lratio[j]<0);
	}
}
void iterbp_layered_awgn(mod2sparse *H, double *lratio, char *dblk)
{
	double t,l,sum,t1;
	mod2entry *e;
	int N,M, i, j,k,sign;
	M = mod2sparse_rows(H);
	N = mod2sparse_cols(H);
	for (i = 0; i < M; i++)
	{
		k=0;
		sum = 0;
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			e->pr = lratio[mod2sparse_col(e)] - e->lr;
			t = log((1+exp(-fabs(e->pr)))/(1-exp(-fabs(e->pr))));
			if(t>50) t=50;	
			sum = sum + t;
			l=(e->pr<0)?1:0;
			k=k+l;
		}  	
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			t1 = log((1+exp(-fabs(e->pr)))/(1-exp(-fabs(e->pr))));
			if(t1>50) t1=50;		
			t = sum - t1;			
			l= (e->pr<0)?1:0;
			sign=((k%2)==l)?1:-1;
			t1 = log((1+exp(-t))/(1-exp(-t)));
			if(t1>50) t1=50;
			e->lr = sign*t1;
		}
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{ 
			j = mod2sparse_col(e);
			lratio[j] = e->pr + e->lr;
		}
	}
	for(j=0;j<N;j++)
	{
		dblk[j] = lratio[j]<0;
	}
}


/***********  BP_flooding_awgn ****************/
unsigned BP_flooding_awgn_dec(mod2sparse *H, double *lratio, char *dblk, char *pchk, int max_iter)
{
	int N,M, n, c;
	int j,i = 0;
	M = mod2sparse_rows(H);
 	N = mod2sparse_cols(H);
	initbp_flooding_awgn(H,lratio,dblk);
	for (n = 0; ; n++)
	{
		c = check(H,dblk,pchk);
		if (n==max_iter || n==-max_iter || (max_iter>0 && c==0))
 		{
			break;
		}
		iterbp_flooding_awgn(H,lratio,dblk);
	}
	return n;
}
void initbp_flooding_awgn(mod2sparse *H, double *lratio, char *dblk)
{
	mod2entry *e;
	int N,j;
	N = mod2sparse_cols(H);
	for (j = 0; j<N; j++)
	{
		for(e = mod2sparse_first_in_col(H,j); !mod2sparse_at_end(e); e = mod2sparse_next_in_col(e))
		{
			e->pr=lratio[j];
			e->lr=0;
		}
		dblk[j] = (lratio[j]<0);
	}
}
void iterbp_flooding_awgn(mod2sparse *H, double *lratio, char *dblk)
{
	double pr,t,l,sum,t1;
	mod2entry *e;
	int N,M, i, j,k,sign;
	M = mod2sparse_rows(H);
	N = mod2sparse_cols(H);
	/* CN. */
	for (i = 0; i < M; i++)
	{
		k=0;
		sum = 0;
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			t = log((1+exp(-fabs(e->pr)))/(1-exp(-fabs(e->pr))));
			if(t>50) t=50;	
			sum = sum + t;
			l=(e->pr<0)?1:0;
			k=k+l;
		}  	
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			t1 = log((1+exp(-fabs(e->pr)))/(1-exp(-fabs(e->pr))));	
			if(t1>50) t1=50;
			t = sum - t1;	
			l= (e->pr<0)?1:0;
			sign=((k%2)==l)?1:-1;
			t1 = log((1+exp(-t))/(1-exp(-t)));
			if(t1>50) t1=50;
			e->lr = sign*t1;	
		}
	}
	/* VN. */  
	for (j = 0; j<N; j++)
	{ 
		pr = lratio[j];
		for (e = mod2sparse_first_in_col(H,j);!mod2sparse_at_end(e);e = mod2sparse_next_in_col(e))
		{ 
			e->pr = pr;
			pr+=e->lr;
		}
		dblk[j] = pr<0;
		pr = 0;
		for (e = mod2sparse_last_in_col(H,j);!mod2sparse_at_end(e);e = mod2sparse_prev_in_col(e))
		{ 
			t = e->pr + pr;     
			e->pr=t;
			pr += e->lr;
		}
	}  
}

/***********  stochastic_BP_flooding_awgn ****************/
unsigned stochastic_BP_flooding_awgn(mod2sparse *H,double *lratio,char *dblk,char *pchk,int max_DC)
{
	FILE *lratio_and_word;
	
	//printf("已经进入该解码函数\n");
	mod2entry* e;
	int N,M,n,c;
	int j,i=0;
	M=mod2sparse_rows(H);
	N=mod2sparse_cols(H);
	double r,temp,R_1;
	int counter[N];  //N个饱和计数器用于硬判决
	int dv[N];
	int seed[4]={5,6,7,8};
	//rand_seed(7*seed+3);
	//double r1,r2,r3,r4;
	node *vn_node;
	for(j=0;j<N;j++)
	{
		dblk[j]=(lratio[j]<0);
		//printf("%d\tlratio=\t%f\n",j,lratio[j]);
		counter[j]=0;
	}
	c=check(H,dblk,pchk);
	for(j=0;j<N;j++)
	{
		dv[j]=mod2sparse_count_col(H,j);
	}
	n=init_stochastic_bp_flooding_awgn(H,lratio,counter,dv);

	lratio_and_word = fopen("test.dat","a");
	
	// printf("dblk:\n ");
	for(j=0;j<N;j++)
	{
		fprintf(lratio_and_word,"%.3f\t%d\n",lratio[j],dblk[j]);
		//printf("%d\n",dblk[j]);
	}
	fclose(lratio_and_word);

	
	if(c==0)
	{
		return n;
	}
	
	vn_node=init_node(dv,N);
	//printf("P=%f\n",*(vn_node->n2[20].P));
	//printf("n1_num=%d\nn2_num=%d\nn3_num=%d\nn4_num=%d\n",vn_node->n1_num,vn_node->n2_num,vn_node->n3_num,vn_node->n4_num);
	//printf("开始进入迭代\n");
	for(;;n++)
	{
		//printf("开始迭代\n");
		if(n==max_DC||n==-max_DC||((max_DC>0)&&(c==0)))
		{
			//printf("n=%d\n",n);
			//printf("c=%d\n",c);
			del_node(vn_node);
			//printf("dblk:\n ");
			// for(j=0;j<N;j++)
			// {
			// printf("%d\n",dblk[j]);
			// }
			break;
		}
		else if(c>0)
		{
			//printf("迭代函数运行\n");
			// r1=rand_uniopen();
			// printf("r1=%f\n",r1);
			// r2=rand_uniopen();
			// //printf("r2=%f\n",r2);
			// r3=rand_uniopen();
			// r4=rand_uniopen();
			iter_stochastic_bp_flooding_awgn(H,lratio,dblk,counter,vn_node,seed);
			seed[0]=seed[0]+2;
			seed[1]=seed[1]+2;
			// if(seed[3]=50)
			// {
			// 	seed[0]=3;
			// 	seed[1]=4;
			// 	seed[2]=5;
			// 	seed[3]=6;
			// }
		}
		// else if(c>0&&c<=20)
		// {
		// 	post_hard_decision(H,dblk,pchk,counter);
			
		// }
		c=check(H,dblk,pchk);
		//printf("c=%d\n",c);
		// printf("dblk:\n ");
		// for(j=0;j<N;j++)
		// {
		// 	printf("%d\n",dblk[j]);
		// }
		// printf("\n");
	}
	return n;
}

void post_hard_decision(mod2sparse *H,char *dblk,char *pchk,int *counter)
{
	mod2entry* e;
	int M,N,i,j;
	M=mod2sparse_rows(H);
	N=mod2sparse_cols(H);

	for(j=0;j<N;j++)
	{
		counter[j]=0;
	}
	mod2sparse_mulvec (H, dblk, pchk);
	for(i=0;i<M;i++)
	{
		if(pchk[i]==1)
		{
			for (e = mod2sparse_first_in_row(H,i);!mod2sparse_at_end(e);e = mod2sparse_next_in_row(e))
			{
				j=mod2sparse_col(e);
				 counter[j]=counter[j]+1;
			}
		}
	}
	for(j=0;j<N;j++)
	{
		if(counter[j]>2)
		{
			dblk[j]=dblk[j]^1;
		}
	}
}

int init_stochastic_bp_flooding_awgn(mod2sparse *H,double *lratio,int *counter,int *dv)
{

	//printf("译码初始化\n");
	mod2entry *e;
	int N,j;
	N=mod2sparse_cols(H);
	for(j=0;j<N;j++)
	{
		for(e = mod2sparse_first_in_col(H,j); !mod2sparse_at_end(e); e = mod2sparse_next_in_col(e))
		{
			e->v2c=(lratio[j]<0);
			e->c2v=0;
		}
		if(lratio[j]<0)
		{
			counter[j]=counter[j]+1;
		}
		else
		{
			counter[j]=counter[j]-1;
		}
	}
	
	return 1;
}

void iter_stochastic_bp_flooding_awgn(mod2sparse *H,double *lratio,char *dblk,int *counter,node *vn_node,int *seed)
{
	//printf("已经进入迭代函数\n");
	mod2entry *e;
	int N,M,i,j;
	M=mod2sparse_rows(H);
	N=mod2sparse_cols(H);
	int k;
	double temp,R;
	int chn[N];
	FILE *c2vmessage;
	c2vmessage=fopen("c2vmessage.dat","a");
	/* stochastic_CN */
	for(i=0;i<M;i++)
	{
		
		k=0;
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			//printf("%d v2c=%d\n",i,e->v2c);
			k=F_proc_stochastic(k,e->v2c);
		}
		for(e = mod2sparse_first_in_row(H,i); !mod2sparse_at_end(e); e = mod2sparse_next_in_row(e))
		{
			e->c2v=F_proc_stochastic(k,e->v2c);
		}
	}
	fclose(c2vmessage);
	//printf("good CN\n");

	/* stochastic_VN */
	rand_seed(seed[0]*7+3);
	double r1;
	for(j=0;j<N;j++)
	{
		r1=rand_uniopen();
		//printf("%d\tr1=%f\n",j,r1);
		temp=1/r1-1;
		R=log(temp)/(4.0*0.6);
		//printf("%d\tR=%f\n",j,R);
		chn[j]=(lratio[j]<R);

		//printf("%d chn=%d\n",j,chn[j]);
		
		if(j>=0&&j<=608)
		{
			i=0;
			for(e = mod2sparse_first_in_col(H,j); !mod2sparse_at_end(e); e = mod2sparse_next_in_col(e))
			{
				vn_node->n4[j].in[i]=e->c2v;
				vn_node->n4[j].chn=chn[j];
				i=i+1;	
			}
		}
		else if(j>=609&&j<=629)
		{
			i=0;
			for(e = mod2sparse_first_in_col(H,j); !mod2sparse_at_end(e); e = mod2sparse_next_in_col(e))
			{
				
				vn_node->n3[j-609].in[i]=e->c2v;
				vn_node->n3[j-609].chn=chn[j];
				i=i+1;
			}
		}
		else if(j>=630&&j<=650)
		{
			i=0;
			for(e = mod2sparse_first_in_col(H,j); !mod2sparse_at_end(e); e = mod2sparse_next_in_col(e))
			{
				
				vn_node->n2[j-630].in[i]=e->c2v;
				vn_node->n2[j-630].chn=chn[j];
				i=i+1;
			}
		}
		else if(j>=651&&j<=671)
		{
			
			e = mod2sparse_first_in_col(H,j);
			vn_node->n1[j-651].in=e->c2v;
			vn_node->n1[j-651].chn=chn[j];		
		}
		
	}
	//printf("input end\n");

	compute_vn_node(vn_node,seed[1],seed[2],seed[3]);
	for(j=0;j<N;j++)
	{
		if(j>=0&&j<=608)
		{
			i=0;
			for (e = mod2sparse_last_in_col(H,j);!mod2sparse_at_end(e);e = mod2sparse_prev_in_col(e))
			{
				e->v2c=vn_node->n4[j].out[i];
				i=i+1;
			}
			//printf("%d i=%d\n",j,i);
			if(e->c2v==e->v2c)
			{
				if(e->v2c==1&&counter[j]<4)
				{
					counter[j]=counter[j]+1;
				}
				else if(e->v2c==0&&counter[j]>-3)
				{
					counter[j]=counter[j]-1;
				}
			}
			// else
			// {
			// 	if(r1>0.5)
			// 	{
			// 		if(counter[j]<4)
			// 		{
			// 			counter[j]=counter[j]+1;
			// 		}
			// 		else if(counter[j]>-3)
			// 		{
			// 			counter[j]=counter[j]-1;
			// 		}
			// 	}
			// }
		}
		else if(j>=609&&j<=629)
		{
			i=0;
			for (e = mod2sparse_last_in_col(H,j);!mod2sparse_at_end(e);e = mod2sparse_prev_in_col(e))
			{
				e->v2c=vn_node->n3[j-609].out[i];
				i=i+1;
			}
			//printf("%d i=%d\n",j,i);
			if(e->c2v==e->v2c)
			{
				if(e->v2c==1&&counter[j]<4)
				{
					counter[j]=counter[j]+1;
				}
				else if(e->v2c==0&&counter[j]>-3)
				{
					counter[j]=counter[j]-1;
				}
			}
			// else
			// {
				
			// 	if(r1>0.5)
			// 	{
			// 		if(counter[j]<4)
			// 		{
			// 			counter[j]=counter[j]+1;
			// 		}
			// 		else if(counter[j]>-3)
			// 		{
			// 			counter[j]=counter[j]-1;
			// 		}
			// 	}
			// }
		}
		else if(j>=630&&j<=650)
		{
			i=0;
			for (e = mod2sparse_last_in_col(H,j);!mod2sparse_at_end(e);e = mod2sparse_prev_in_col(e))
			{
				e->v2c=vn_node->n2[j-630].out[i];
				i=i+1;
			}
			//printf("%d i=%d\n",j,i);
			if(e->c2v==e->v2c)
			{
				if(e->v2c==1&&counter[j]<4)
				{
					counter[j]=counter[j]+1;
				}
				else if(e->v2c==0&&counter[j]>-3)
				{
					counter[j]=counter[j]-1;
				}
			}
			// else
			// {
			// 	//r=rand_uniopen();
			// 	if(r1>0.5)
			// 	{
			// 		if(counter[j]<4)
			// 		{
			// 			counter[j]=counter[j]+1;
			// 		}
			// 		else if(counter[j]>-3)
			// 		{
			// 			counter[j]=counter[j]-1;
			// 		}
			// 	}
			// }
		}
		else if(j>=651&&j<=671)
		{
			e = mod2sparse_first_in_col(H,j);
			e->v2c=chn[j];
			if(e->c2v==e->v2c)
			{
				if(e->v2c==1&&counter[j]<4)
				{
					counter[j]=counter[j]+1;
				}
				else if(e->v2c==0&&counter[j]>-3)
				{
					counter[j]=counter[j]-1;
				}
			}
			// else
			// {
			// 	// r=rand_uniopen();
			// 	if(r1>0.5)
			// 	{
			// 		if(counter[j]<4)
			// 		{
			// 			counter[j]=counter[j]+1;
			// 		}
			// 		else if(counter[j]>-3)
			// 		{
			// 			counter[j]=counter[j]-1;
			// 		}
			// 	}
			// }
		}
		dblk[j]=(counter[j]>0);
	}
	
}

int F_proc_stochastic(int in1,int in2)//F(x,y)=x*(1-y)+(1-x)*y
{
	int result;
	result=in1^in2;
	return result;
}

void G_TFM2(int in1,int in2,int *result,double *P,double beta,int seed)//G(x,y)=(x*y)/(x*y+(1-x)*(1-y))with TFM
{
	double R;
	rand_seed(10*seed+3);
	printf("seed=%d\n",seed);

	//printf("beta=%lf\n",beta);
	
	if(in1==in2)
	{
		*P=*P+beta*(in1-*P);
	}
	double tmp=*P;
	//printf("P=%f\n",*P);
	
	R=rand_uniopen();
	//printf("R=%f\n",R);
	if(in1==in2)
		*result=in1;
	else{
		if(tmp>=R)
			*result=1;
		else
			*result=0;
	}
}
void G_TFM(int in1,int in2,int *result,double *P,double beta,double r)//G(x,y)=(x*y)/(x*y+(1-x)*(1-y))with TFM
{
	//printf("beta=%lf\n",beta);
	
	if(in1==in2)
	{
		*P=*P+beta*(in1-*P);
	}
	double tmp=*P;
	
	
	if(in1==in2)
		*result=in1;
	else{
		if(tmp>=r)
			*result=1;
		else
			*result=0;
	}
}

node *init_node(int *dv,int N)
{
	node *vn_node;
	vn_node=(node *)malloc(sizeof(node));
	//printf("结点初始化\n");
	int j;
	vn_node->n1_num=0;
	vn_node->n2_num=0;
	vn_node->n3_num=0;
	vn_node->n4_num=0;
	for(j=0;j<N;j++)
	{
		switch (dv[j])
		{
		case 1:{
			vn_node->n1_num=vn_node->n1_num+1;
			break;
		}
		case 2:{
			vn_node->n2_num=vn_node->n2_num+1;
			break;
		}
		case 3:{
			vn_node->n3_num=vn_node->n3_num+1;
			break;
		}
		case 4:{
			vn_node->n4_num=vn_node->n4_num+1;
			break;
		}
		default:
			break;
		}
	}

	//printf("n1_num=%d\nn2_num=%d\nn3_num=%d\nn4_num=%d\n",vn_node->n1_num,vn_node->n2_num,vn_node->n3_num,vn_node->n4_num);

	vn_node->n1=NULL;
	vn_node->n2=NULL;
	vn_node->n3=NULL;
	vn_node->n4=NULL;
	vn_node->n1=(node1*)malloc(sizeof(node1)*vn_node->n1_num);
	vn_node->n2=(node2*)malloc(sizeof(node2)*vn_node->n2_num);
	vn_node->n3=(node3*)malloc(sizeof(node3)*vn_node->n3_num);
	vn_node->n4=(node4*)malloc(sizeof(node4)*vn_node->n4_num);

	if(vn_node->n1==NULL)
	{
		printf("out of memory\n");
	}
	if(vn_node->n2==NULL)
	{
		printf("out of memory\n");
	}
	if(vn_node->n3==NULL)
	{
		printf("out of memory\n");
	}
	if(vn_node->n4==NULL)
	{
		printf("out of memory\n");
	}
	
	for(j=0;j<vn_node->n2_num;j++)
	{
		vn_node->n2[j].P=(double *)malloc(sizeof(double)*2);
		vn_node->n2[j].P[0]=0.5;
		vn_node->n2[j].P[1]=0.5;
	}
	
	for(j=0;j<vn_node->n3_num;j++)
	{
		vn_node->n3[j].P=(double *)malloc(sizeof(double)*5);
		vn_node->n3[j].P[0]=0.5;
		vn_node->n3[j].P[1]=0.5;
		vn_node->n3[j].P[2]=0.5;
		vn_node->n3[j].P[3]=0.5;
		vn_node->n3[j].P[4]=0.5;
	}
	for(j=0;j<vn_node->n4_num;j++)
	{
		vn_node->n4[j].P=(double *)malloc(sizeof(double)*8);
		vn_node->n4[j].P[0]=0.5;
		vn_node->n4[j].P[1]=0.5;
		vn_node->n4[j].P[2]=0.5;
		vn_node->n4[j].P[3]=0.5;
		vn_node->n4[j].P[4]=0.5;
		vn_node->n4[j].P[5]=0.5;
		vn_node->n4[j].P[6]=0.5;
		vn_node->n4[j].P[7]=0.5;
	}
	return vn_node;
}

void compute_vn_node(node *vn_node,int seed2,int seed3,int seed4)
{
	int i;
	
	//printf("n1_num=%d\nn2_num=%d\nn3_num=%d\nn4_num=%d\n",vn_node->n1_num,vn_node->n2_num,vn_node->n3_num,vn_node->n4_num);
	double beta=pow(2,-2);
	//printf("%d\n",vn_node->n3[20].in[0]);
	rand_seed(seed2*7+3);
	double r;
	for(i=0;i<vn_node->n2_num;i++)
	{
		//printf("n2 hello %d\n",i);
		r=rand_uniopen();
		G_TFM(vn_node->n2[i].in[0],vn_node->n2[i].chn,&(vn_node->n2[i].out[0]),&(vn_node->n2[i].P[0]),beta,r);
		r=rand_uniopen();
		G_TFM(vn_node->n2[i].in[1],vn_node->n2[i].chn,&(vn_node->n2[i].out[1]),&(vn_node->n2[i].P[1]),beta,r);
	}
	for(i=0;i<(vn_node->n3_num);i++)
	{
		//printf("n3 hello %d",i);

		int wire[2];
		r=rand_uniopen();
		G_TFM(vn_node->n3[i].in[0],vn_node->n3[i].in[1],&(wire[0]),&(vn_node->n3[i].P[0]),beta,r);
		r=rand_uniopen();
		G_TFM(vn_node->n3[i].in[2],vn_node->n3[i].chn,&(wire[1]),&(vn_node->n3[i].P[1]),beta,r);
		r=rand_uniopen();
		G_TFM(wire[0],vn_node->n3[i].chn,&(vn_node->n3[i].out[0]),&(vn_node->n3[i].P[2]),beta,r);
		r=rand_uniopen();
		G_TFM(wire[1],vn_node->n3[i].in[0],&(vn_node->n3[i].out[1]),&(vn_node->n3[i].P[3]),beta,r);
		r=rand_uniopen();
		G_TFM(wire[1],vn_node->n3[i].in[1],&(vn_node->n3[i].out[2]),&(vn_node->n3[i].P[4]),beta,r);
	}
	for(i=0;i<vn_node->n4_num;i++)
	{
		//printf("n4 hello %d",i);

		int wire1[2];
		int wire2[2];
		r=rand_uniopen();
		G_TFM(vn_node->n4[i].in[0],vn_node->n4[i].in[1],&(wire1[0]),&(vn_node->n4[i].P[0]),beta,r);
		r=rand_uniopen();
		G_TFM(vn_node->n4[i].in[2],vn_node->n4[i].in[3],&(wire1[1]),&(vn_node->n4[i].P[1]),beta,r);
		r=rand_uniopen();
		G_TFM(vn_node->n4[i].chn,wire1[0],&(wire2[0]),&(vn_node->n4[i].P[2]),beta,r);
		r=rand_uniopen();
		G_TFM(vn_node->n4[i].chn,wire1[1],&(wire2[1]),&(vn_node->n4[i].P[3]),beta,r);
		r=rand_uniopen();
		G_TFM(wire2[0],vn_node->n4[i].in[2],&(vn_node->n4[i].out[0]),&(vn_node->n4[i].P[4]),beta,r);
		r=rand_uniopen();
		G_TFM(wire2[0],vn_node->n4[i].in[3],&(vn_node->n4[i].out[1]),&(vn_node->n4[i].P[5]),beta,r);
		r=rand_uniopen();
		G_TFM(wire2[1],vn_node->n4[i].in[0],&(vn_node->n4[i].out[2]),&(vn_node->n4[i].P[6]),beta,r);
		r=rand_uniopen();
		G_TFM(wire2[1],vn_node->n4[i].in[1],&(vn_node->n4[i].out[3]),&(vn_node->n4[i].P[7]),beta,r);
	}
}

void del_node(node *vn_node)
{
	int j;
	for(j=0;j<vn_node->n2_num;j++)
	{
		free(vn_node->n2[j].P);
	}
	for(j=0;j<vn_node->n3_num;j++)
	{
		free(vn_node->n3[j].P);
	}
	for(j=0;j<vn_node->n4_num;j++)
	{
		free(vn_node->n4[j].P);
	}
	free(vn_node->n1);
	free(vn_node->n2);
	free(vn_node->n3);
	free(vn_node->n4);
}

double rng(int *sequence, int N)
 {
	 double rng_number=0;
	 int i, temp;

	 if(N==11)
		temp = sequence[8] ^ sequence[10];//11bit
	 else if(N==10)
		temp = sequence[6] ^ sequence[9];//10bit
	 else if(N==9)
		temp = sequence[4] ^ sequence[8];//9bit
	 else if(N==8)
		 temp = sequence[3] ^ sequence[4] ^ sequence[5] ^ sequence[7];//8bit
	 else if(N==7)
		 temp = sequence[5] ^ sequence[6];	
	 for (i=N-1;i>0;i--)
	 {
		sequence[i] = sequence[i-1]; 
	 }
	 sequence[0] = temp;
	 
	  for (i=0;i<N;i++)
	 {
		rng_number = rng_number+sequence[i]*pow(2,N-1-i);
		
	 }
	 /* printf("%f %d",rng_number,sequence[i]);
	 getchar(); */
	 rng_number = rng_number/(pow(2,N));
	 /* printf("%f %d",rng_number,sequence[i]);
	 getchar(); */
	 return rng_number;
 }

void dec2vec(int a, int *x, int pow)  //将整数转化为二进制向量，数组的第一个元素为最高位
{
  int i;
  int aCopy = a;
  for (i = 0; i < pow; i++) {
    x[i] = 0;
  }
  for (i = pow-1; i>=0; i--) {
    x[i] = aCopy % 2;
    aCopy = (int) (aCopy / 2);
    if (aCopy == 0) break;
  }
}





























// void init_subnode(subnode *sub_node)
// {
// 	sub_node->beta=pow(2,-2);
// 	*(sub_node->P)=0.5;
// 	sub_node->in1=0;
// 	sub_node->in2=-1;
// 	sub_node->out=0;
// }

// void calculate_subnode(subnode *sub_node)
// {
// 	if((sub_node->in2!=0)||(sub_node->in2!=1))
// 	{
// 		sub_node->out=sub_node->in1;
// 	}
// 	else
// 	{
// 		G_TFM(sub_node->in1,sub_node->in2,&(sub_node->out),sub_node->P,sub_node->beta);
// 	}
// }

// void generate_stage(stage *substage,int in_num)
// {
// 	substage->in_num=in_num; 
// 	substage->out_num=ceil((substage->in_num)/2.0);
// 	substage->in=(int*)malloc(sizeof(int)*(substage->in_num));
// 	substage->out=(int*)malloc(sizeof(int)*(substage->out_num));
// 	substage->sub_node=(subnode*)malloc(sizeof(subnode)*(substage->out_num));
// 	int i;
// 	for(i=0;i<substage->out_num;i++)
// 	{
// 		init_subnode(&(substage->sub_node[i]));
// 	}
// }

// void generate_dual_tree(dual_tree *tree,int dv)
// {
//    tree->in_num=dv+1; //include c2v messages and channel message
//    tree->out_num=dv+1; 
//    tree->S=ceil(log(tree->in_num)/log(2))-1;
//    tree->in=(int*)malloc(sizeof(int)*(tree->in_num));
//    tree->out=(int*)malloc(sizeof(int)*(tree->out_num));
//    tree->substage=(stage*)malloc(sizeof(stage)*(2*(tree->S)));
//    int i,s;
//    for(s=1;s<=(tree->S);s++)
//    {
// 		i=2*ceil((tree->in_num)/pow(2,s));
// 		generate_stage(&(tree->substage[s-1]),i);
//    }
//    for(s=(tree->S);s<=(2*(tree->S));s++)
//    {
// 		i=2*ceil((tree->in_num)/pow(2,2*(tree->S)-s));
// 		generate_stage(&(tree->substage[s-1]),i);
//    }
// }

// void compute_dualtree(dual_tree *tree)
// {
// 	int s,i;
// 	tree->substage[1].in=tree->in;
// 	for(s=2;s<=(tree->S);s++)
// 	{
// 		tree->substage[s].in=tree->substage[s-1].out;
// 		sort_inputs_subnode_1(&(tree->substage[s]));
// 	}
	
// 	for(s=(tree->S+1);s<2*(tree->S);s++)
// 	{
// 		if(s=(tree->S+1))
// 		{
// 			sort_inputs_subnode_2(tree->substage,s,tree->S);
// 		}
// 		else
// 		{
// 			sort_inputs_subnode_3(tree->substage,s,tree->S);
// 		}
// 	}
// }

// void sort_inputs_subnode_1(stage *substage)
// {
// 	int j;
// 	if((substage->in_num%2==0)&&(substage->in_num>0))
// 	{
// 		for(j=0;j<substage->in_num;j=j+2)
// 		{
// 			substage->sub_node[j/2].in1=substage->in[j];
// 			substage->sub_node[j/2].in2=substage->in[j+1];
// 			calculate_subnode(&(substage->sub_node[j]));
// 			substage->out[j/2]=substage->sub_node[j/2].out;
// 		}
// 	}
// 	else if((substage->in_num%2==1)&&(substage->in_num>0))
// 	{
// 		for(j=0;j<substage->in_num;j=j+2)
// 		{
// 			substage->sub_node[j/2].in1=substage->in[j];
// 			substage->sub_node[j/2].in2=substage->in[j+1];
// 			calculate_subnode(&(substage->sub_node[j/2]));
// 			substage->out[j/2]=substage->sub_node[j/2].out;
// 		}
// 		substage->sub_node[substage->out_num-1].in1=substage->in[substage->in_num];
// 		calculate_subnode(&(substage->sub_node[substage->out_num-1]));
// 		substage->out[substage->out_num-1]=substage->sub_node[substage->out_num-1].out;
// 	}
// }

// void sort_inputs_subnode_2(stage *substage,int s,int S)
// {
// 	int t,k;
// 	for(t=0;t<substage[s].out_num;t++)
// 	{
// 		k=floor((-t)/2.0)+1;
// 		substage[s].sub_node[t].in1=substage[s-1].out[k];
// 		k=t+pow(-1,t);
// 		substage[s].sub_node[t].in2=substage[2*S-s].out[k];
// 		calculate_subnode(&(substage[s].sub_node[t]));
// 		substage[s].out[t]=substage[s].sub_node[t].out;
// 	}
// }

// void sort_inputs_subnode_3(stage *substage,int s,int S)
// {
// 	int t,k;
// 	if(((substage[s].in_num)%2==0)&&(substage[s].in_num)>0)
// 	{
// 		for(t=0;t<substage[s].out_num;t++)
// 		{
// 			k=floor((t)/2.0);
// 			substage[s].sub_node[t].in1=substage[s-1].out[k];
// 			k=t+pow(-1,t);
// 			substage[s].sub_node[t].in2=substage[2*S-s].out[k];
// 			calculate_subnode(&(substage[s].sub_node[t]));
// 			substage[s].out[t]=substage[s].sub_node[t].out;
// 		}
// 	}
// 	else if(((substage[s].in_num)%2==1)&&(substage[s].in_num>0))
// 	{
// 		for(t=0;t<substage[s].out_num-1;t++)
// 		{
// 			k=floor((t)/2.0);
// 			substage[s].sub_node[t].in1=substage[s-1].out[k];
// 			k=t+pow(-1,t);
// 			substage[s].sub_node[t].in2=substage[2*S-s].out[k];
// 			calculate_subnode(&(substage[s].sub_node[t]));
// 			substage[s].out[t]=substage[s].sub_node[t].out;
// 		}
// 		substage[s].out[substage[s].out_num-1]=substage[s-1].out[substage[s-1].out_num-1];
// 	}
// }



// void del_node(subnode *sub_node)
// {
// 	if(sub_node!=NULL)
// 	{
// 		free(sub_node);
// 	}
// }
// void del_stage(stage *substage)
// {
// 	int i;
// 	if(substage->sub_node!=NULL)
// 	{
// 		for(i=0;i<substage->out_num;i++)
// 		{
// 			{
// 				del_node(&(substage->sub_node[i]));
// 			}
// 		}
// 	}
// 	free(substage);
// }
// void del_dualtree(dual_tree *tree)
// {
// 	int i;
// 	if(tree->substage!=NULL)
// 	{
// 		for(i=0;i<2*(tree->S);i++)
// 		{
// 			{
// 				del_stage(&(tree->substage[i]));
// 			}
// 		}
// 	}
// }






































// unsigned stochastic_BP_flloding_awgn(mod2sparse *H, double *lratio, char *dblk, char *pchk, int max_iter,int stochastic_len)
// {
// 	int N,M, n, c;
// 	int j,i = 0;
// 	int**** T;
// 	int** t; //t[i][0]为VN的degree
// 	double*** P;
// 	int** bitstream_chn; //信道消息转换
// 	M = mod2sparse_rows(H);
//  	N = mod2sparse_cols(H);
// 	init_stochastic_bp_flooding_awgn(H,lratio,dblk,stochastic_len,T,t,bitstream_chn);
// 	for (n = 0; ; n++)
// 	{
// 		c = check(H,dblk,pchk);
// 		if (n==max_iter || n==-max_iter || (max_iter>0 && c==0))
//  		{
// 			break;
// 		}
// 		iter_stochastic_bp_flooding_awgn(H,dblk,stochastic_len,T,t,bitstream_chn,P);
// 	}
// 	return n;
// }

// void init_stochastic_bp_flooding_awgn(mod2sparse *H, double *lratio, char *dblk,int stochastic_len,int**** T,int** t,double*** P,int** bitstream_chn)
// {
// 	mod2entry *e;
// 	int N,j,i;
// 	N = mod2sparse_cols(H);
// 	T=(int****)malloc(sizeof(int***)*N);
// 	t=(int**)malloc(sizeof(int*)*N);
// 	P=(double***)maloc(sizeof(double**)*N);
// 	double r,temp,R_1;
// 	bitstream_chn=(int**)malloc(sizeof(int*)*N);
// 	for(j=0;j<N;j++)
// 	{
// 		dblk[j]=(lratio[j]<0);
// 		bitstream_chn[j]=(int*)malloc(sizeof(int)*stochastic_len);
// 		for(i=0;i<stochastic_len;i++)
// 		{
// 			r = rand()/(RAND_MAX+1);
// 			temp = 1/r-1;
// 			R_1=log(temp);
// 			if(lratio[j]<=R_1)
// 			{
// 				bitstream_chn[j][i]=1;
// 			}
// 			else bitstream_chn[j][i]=0;
// 		}
// 		i=0;
// 		for(e=mod2sparese_first_in_col(H,j);!mod2sparse_at_end(e);e=mod2sparse_next_in_col(e))
// 		{
// 			e->pr=lratio[j];
// 			e->lr=0; 
// 			e->bitstream_c2v=(int*)calloc(stochastic_len,sizeof(int));  //calloc分配地址的同时每个元素赋值为0
// 			e->bitstream_v2c=(int*)malloc(sizeof(int)*stochastic_len);
// 			e->bitstream_v2c=bitstream_chn[j];
// 			i=i+1;
// 		}
// 		i=i+1; //考虑到VNU的输入还需要有信道消息v
// 		generate_dual_tree(stochastic_len,i,T[j],t[j],P[j]);
// 	}
// }

// int F_proc_stochastic(int in1,int in2)//F(x,y)=x*(1-y)+(1-x)*y
// {
// 	int result;
// 	result=in1^in2;
// 	return result;
// }

// int G_proc_stochastic(int in1,int in2,int Q)//G(x,y)=(x*y)/(x*y+(1-x)*(1-y))
// {
// 	//printf("get in G_proc\n");
// 	int result;
// 	int J,K;
// 	J=in1*in2;
// 	K=(1-in1)*(1-in2);
// 	result=J*(1-Q)+(1-K)*Q;
// 	//printf("result=%d\n",result);
// 	return result;
// }

// void G_TFM(int in1,int in2,int *result,double* P,int k,double beta)//G(x,y)=(x*y)/(x*y+(1-x)*(1-y))with TFM
// {
// 	double R;
	
// 	//printf("beta=%lf\n",beta);
	
// 	if(in1==in2)
// 		P[k]=P[k]+beta*(in1-P[k]);

// 	double tmp=P[k];
	
// 	R=rand()/(RAND_MAX+1.0);
// 	if(in1==in2)
// 		*result=in1;
// 	else{
// 		if(tmp>=R)
// 			*result=1;
// 		else
// 			*result=0;
// 	}
	
// }

// /* 2-input/1-output subnode */
// void subnode(int* in1,int* in2,int* result,int stochastic_len,double* P,int k)
// {
// 	double beta=pow(2,-2);
// 	int i;
// 	for(i=0;i<stochastic_len;i++)
// 	{
// 		G_TFM(in1[i],in2[i],result[i],P,k,beta);
// 	}
// }

// /* create the dual tree for VN */
// void generate_dual_tree(int stochastic_len,int I,int*** T,int* t,double** P)
// {
// 	mod2entry *e;
// 	int S=ceil(log(I)/log(2))-1;
// 	int i,j;
// 	t=(int*)malloc(sizeof(int)*(2*S+1));
// 	int sum_wire=t[0]=t[2*S]=I;
	
// 	//求解每级需要多少导线
// 	for(i=1;i<S+1;i++)
// 	{
// 		t[i]=ceil(double(t[i-1])/2);
// 		sum_wire=sum_wire+t[i];
// 	}
// 	for(i=2*S-1;i>S;i--)
// 	{
// 		t[i]=ceil(double(t[i+1])/2);
// 		sum_wire=sum_wire+t[i];
// 	}

// 	T=(int***)malloc(sizeof(int**)*(2*S+1));
// 	P=(double**)malloc(sizeof(double*)*(2*S+1));
	
// 	for(i=0;i<2*S;i++)
// 	{
// 		T[i]=(int**)malloc(sizeof(int*)*t[i]);
// 		P[i]=(double*)malloc(sizeof(double)*t[i]);
// 	}
// 	for(i=0;i<2*S;i++)
// 	{
// 		for(j=0;j<t[i];j++)
// 		{
// 			T[i][j]=(int*)malloc(sizeof(int)*stochastic_len);
// 			P[i][j]=0.5;
// 		}
// 	}
// }

// /* compute in dual_tree */
// void compute_dual_tree(int*** T,int* t,int stochastic_len,double** P)
// {
// 	int S=ceil(log(t[0])/log(2))-1;
// 	int s,i;
// 	for(s=1;s<=S;s++)
// 	{
// 		for(i=0;i<t[s];i++)
// 		{
// 			subnode(T[s-1][2*i],T[s-1][2*i+1],T[s][i],stochastic_len,P[s],i);
// 		}
// 	}
// }

// /* delete dual_tree */
// void del_dual_tree(int*** T)
// {

// }

// void iter_stochastic_bp_flooding_awgn(mod2sparse *H, char *dblk,int stochastic_len,int**** T,int** t,int** bitstream_chn,double*** P)
// {
// 	double pr,l,sum,t1;
// 	mod2entry *e;
// 	int N,M, i, j,k,sign;
// 	int* p_temp=(int*)malloc(sizeof(int)*stochastic_len);
// 	M = mod2sparse_rows(H);
// 	N = mod2sparse_cols(H);
// 	/*stochastic CNU*/
// 	for(i=0;i<M;i++)
// 	{
// 		for(i=0;i<stochastic_len;i++)
// 		{
// 			p_temp[i]=0;
// 		}	
// 		for(e=mod2sparse_first_in_row(H,i);!mod2sparse_at_end(e);e=mod2sparse_next_in_row(e))
// 		{
// 			for(i=0;i<stochastic_len;i++)
// 			{
// 				p_temp[i]=F_proc_stochastic(p_temp[i],e->bitstream_v2c[i]);
// 			}
// 		}
// 		for(e=mod2sparse_first_in_row(H,i);!mod2sparse_at_end(e);e=mod2sparse_next_in_row(e))
// 		{
// 			for(i=0;i<stochastic_len;i++)
// 			{
// 				e->bitstream_c2v[i]=F_proc_stochastic(p_temp[i],e->bitstream_v2c[i]);
// 			}
// 		}
// 	}
	

// 	/* stochastic_VN */
// 	for(j=0;j<N;j++)
// 	{
// 		for (k=0,e = mod2sparse_first_in_col(H,j);!mod2sparse_at_end(e);e = mod2sparse_next_in_col(e),k++)
// 		{
// 			T[j][0][k]=e->bitstream_c2v;
// 		}
// 		k=k+1;
// 		T[j][0][k]=bitstream_chn[j];
// 		compute_dual_tree(T[j],t[j],stochastic_len,P[j]);
// 	}

	 
// }