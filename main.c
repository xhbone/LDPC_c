#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "mod2sparse.h"
#include "dec.h"
#include "rcode.h"
#include "rand.h"


int main
( int argc,
  char **argv
)
{
	typedef enum { BSC, AWGN } channel_type;
	channel_type channel;	/* Type of channel */
	double error_prob;	/* Error probability for BSC */
	double SNR;		/* Signal Noise ratio for AWGN */
	char *pchk_file, *file;
	FILE *err,*result,*code_fp, *NonMatch_file;
	int seed =  5;
	int check_begin;
	int i,j,k;
	char *dblk,*pchk;
	//int *codeword_temp;
	int *codeword;
	char *dec_word;
	int bsc_noise;
	double *lratio,*lratio1;
	double std_dev,awn_noise,awn_data;
	int max_iter,max_err,max_block;
	double tot_iter;
	int block_no,localerror,errorbit,errorblo;
	unsigned iter;
	if (argc != 10) {
		printf("the number of input variable is wrong; reqire 10");
	}
	pchk_file = argv[1];	/* parity check matrix */
	check_begin = atoi(argv[2]);	/* the point to start information bits */
	file = argv[3];		/* the file to store result */
	
	/* ***********     select channel  ****************** */
	if(strcmp(argv[4],"awgn")==0 || strcmp(argv[4],"AWGN")==0){
		channel = AWGN;
		SNR = atof(argv[5]); 
	}
	else if(strcmp(argv[4],"bsc")==0 || strcmp(argv[4],"BSC")==0){
		channel = BSC;
		error_prob = atof(argv[5]); 
	}
	else{
		printf("unexpect channel mode");			
	} 
	/* **********    select decoding method *********** */
	if (strcmp(argv[6],"MS_layered_awgn")==0){
		dec_method = MS_layered_awgn;
	}
	else if (strcmp(argv[6],"MS_flooding_awgn")==0){
		dec_method = MS_flooding_awgn;
	}
	else if (strcmp(argv[6],"MS_flooding_awgn_orig")==0){
		dec_method = MS_flooding_awgn_orig;
	}
	else if (strcmp(argv[6],"OMS_layered_awgn")==0){
		dec_method = OMS_layered_awgn;
	}
	else if (strcmp(argv[6],"OMS_flooding_awgn")==0){
		dec_method = OMS_flooding_awgn;
	}
	else if (strcmp(argv[6],"BP_layered_awgn")==0){
		dec_method = BP_layered_awgn;
	}	
	else if (strcmp(argv[6],"BP_flooding_awgn")==0){
		dec_method = BP_flooding_awgn;
	}
	else if(strcmp(argv[6],"stochastic_BP_awgn")==0){
		dec_method = stochastic_BP_awgn;
	}
	else{
		printf("unexpect decoding method");	
	} 
	max_iter = atoi(argv[7]);
	max_err = atoi(argv[8]);
	max_block = atoi(argv[9]);

	read_pchk(pchk_file); 	/* Read parity check file. */
	rand_seed(10*seed+3);
	err=fopen("errorout.g","wb");

	
	std_dev = sqrt(1/(2*((double)(N-M))/N*pow(10, SNR/10)));
	dec_word = (char *)malloc( N * sizeof(char) );	
	lratio = (double *)malloc( N * sizeof(double) );
	lratio1 = (double *)malloc( N * sizeof(double) );
	dblk = (char *)malloc( N * sizeof(char) );
	pchk = (char *)malloc( M * sizeof(char) );
	printf("M=%d\n",M);
	printf("N=%d\n",N);
	tot_iter = 0;
	errorbit = 0;
	errorblo = 0;
	int rank,Z = 52;
	//code_fp = fopen(argv[10],"r");
	//code_fp = fopen("codeword_wimax_12_2304.txt","r");
	//code_fp = fopen("codeword_dv3R075N1296.txt","r");
    code_fp = fopen("codeword_irregular_588_672.txt","r");
	//code_fp = fopen("codeword_irregular_504_672.txt","r");
	// code_fp = fopen("codeword_irregular_336_672.txt","r");
	//code_fp = fopen("codeword_regular_R080_4_20_512.txt","r");
	//code_fp = fopen("codeword_BG2K520Z52N2600.txt","r");
	printf("dec_method=%d\n",dec_method);
    codeword = (int *)malloc( N * sizeof(int) );
	
for(i = 0;i< N;i++)
 {
//  codeword[i] = 0;
j =  fscanf(code_fp,"%d", &codeword[i]);
 }
 fclose(code_fp); 
	  
	  
	for(block_no = 0;  ; block_no++){
		switch (channel){ 
			case BSC:{ 
				
				for(i=0;i<N;i++){
					bsc_noise = (rand_uniform)() < error_prob;
					dec_word[i] = codeword[i]^bsc_noise;
					lratio[i] = log((1-error_prob)/error_prob);
					lratio[i] = (dec_word[i] == 0) ? lratio[i] : -lratio[i];
					}
				break;
			}
			case AWGN:{ 
					
				for(i=0;i<N;i++){
					awn_noise = std_dev*rand_gaussian();
					awn_data = (codeword[i] == 0) ? 1+awn_noise : -1+awn_noise;
					lratio[i] = 2*awn_data/(std_dev*std_dev);
					lratio1[i] = 2*awn_data/(std_dev*std_dev);
					// lratio1[i] = awn_data;
					dec_word[i] = awn_data < 0;
				}			
				break;
			}
			default:{
				printf("unexpect channel mode");	
			}
		}
		switch (dec_method){
			case MS_layered_awgn:{ 
				iter = MS_layered_awgn_dec (H, lratio1, dblk, pchk, max_iter);
				break;
			}
			case MS_flooding_awgn_orig:{ 
				iter = MS_flooding_awgn_dec_orig (H, lratio1, dblk, pchk, max_iter);
				break;
			}
			case OMS_layered_awgn:{ 
				iter = OMS_layered_awgn_dec (H, lratio1, dblk, pchk, max_iter);
				break;
			}
			case OMS_flooding_awgn:{ 
				iter = OMS_flooding_awgn_dec (H, lratio1, dblk, pchk, max_iter);
				break;
			}
			case BP_layered_awgn:{ 
				iter = BP_layered_awgn_dec (H, lratio, dblk, pchk, max_iter);
				break;
			}
			case BP_flooding_awgn:{ 
				iter = BP_flooding_awgn_dec(H, lratio, dblk, pchk, max_iter);
				break;
			}
			case stochastic_BP_awgn:{
				iter = stochastic_BP_flooding_awgn(H,lratio,dblk,pchk,max_iter);
				break;
			}
			default:{
				printf("unexpect decoding method");	
			}
		}
		tot_iter+=iter;
		localerror=0;
/* 		for(i=0;i<N;i++){
			if(lratio[i] == 0){
			dblk[i] = dec_word[i];
			}
		} */

		for(i=0;i<N;i++){
			if(dblk[i] != codeword[i]){
				errorbit+=1;
				localerror+=1;
				//fprintf(err," %d %+5.2f",i,awn_data[i]);
			}
		}
		NonMatch_file = fopen("./NonMatch.dat", "a");
		if(localerror!=0) {
			errorblo+=1;			
			fprintf(NonMatch_file, "%s\n", "Bad Frames!!!");
		}
		fclose(NonMatch_file);
		// print BER/FER
		if(block_no%500000==0 && block_no){
			printf("block %d error block %d errorbit %d.\n ",block_no+1,errorblo,errorbit);
			printf("Block error rate: %.3e\n", (double)errorblo/((double)(block_no+1)));
		}
		 if(errorblo==max_err || block_no == max_block){
		//if(block_no == 10){
			fclose(err);
			result=fopen(file,"a"); 
			printf("Decoded %d blocks, %d valid.  Average %.1f iterations\n",block_no+1, block_no+1-errorblo, (double)tot_iter/(block_no+1));
			printf("Block error rate: %.3e\n", (double)errorblo/((double)(block_no+1)));
			printf("Bit error rate: %.3e\n", (double)errorbit/((double)(block_no+1)*(double)(N)) );
			fprintf(result,"%.3f %.3f %.3e %4.1f %.3e  %s %s\n",SNR,error_prob,(double)errorbit/((double)(block_no+1)*(double)(N)),(double)tot_iter/(block_no+1),(double)errorblo/(block_no+1),argv[6],argv[11]);
			fclose(result);		
			break;
		}
	}
	
	return(0);
}

