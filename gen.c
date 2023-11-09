#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rand.h"

void matrixGenBG2(
	int CodeLength,
	int InfoLength,
	int Circulant,
	int **H)
{
	double kb;
	kb = (double)InfoLength / (double)Circulant;
	int M_base_max = 42;
	int N_base_max = 52;
	int H_base[M_base_max][N_base_max];
	int K_base;
	K_base = kb;
	if (kb > K_base)
		K_base = kb + 1;
	if (K_base < 6 || K_base > 10)
		printf("wrong pair：InfoLength and Circulant");

	FILE *base_matrix;
	if ((Circulant == 2) || (Circulant == 4) || (Circulant == 8) || (Circulant == 16) || (Circulant == 32) || (Circulant == 64) || (Circulant == 128) || (Circulant == 256))
	{
		base_matrix = fopen("BG2_2.txt", "r");
	}
	else if ((Circulant == 3) || (Circulant == 6) || (Circulant == 12) || (Circulant == 24) || (Circulant == 48) || (Circulant == 96) || (Circulant == 192))
	{
		base_matrix = fopen("BG2_3.txt", "r");
	}
	else if ((Circulant == 5) || (Circulant == 10) || (Circulant == 20) || (Circulant == 40) || (Circulant == 80) || (Circulant == 160))
	{
		base_matrix = fopen("BG2_5.txt", "r");
	}
	else if ((Circulant == 7) || (Circulant == 14) || (Circulant == 28) || (Circulant == 56) || (Circulant == 112) || (Circulant == 224))
	{
		base_matrix = fopen("BG2_7.txt", "r");
	}
	else if ((Circulant == 9) || (Circulant == 18) || (Circulant == 36) || (Circulant == 72) || (Circulant == 144))
	{
		base_matrix = fopen("BG2_9.txt", "r");
	}
	else if ((Circulant == 11) || (Circulant == 22) || (Circulant == 44) || (Circulant == 88) || (Circulant == 176))
	{
		base_matrix = fopen("BG2_11.txt", "r");
	}
	else if ((Circulant == 13) || (Circulant == 26) || (Circulant == 52) || (Circulant == 104) || (Circulant == 208))
	{
		base_matrix = fopen("BG2_13.txt", "r");
	}
	else if ((Circulant == 15) || (Circulant == 30) || (Circulant == 60) || (Circulant == 120) || (Circulant == 240))
	{
		base_matrix = fopen("BG2_15.txt", "r");
	}
	else
	{
		printf("wrong Circulant");
	}
	int i, j, k;
	for (i = 0; i < M_base_max; i++)
	{
		for (j = 0; j < N_base_max; j++)
		{
			k = fscanf(base_matrix, "%d", &H_base[i][j]);
		}
	}
	fclose(base_matrix);
	int M, N;
	N = CodeLength;
	M = CodeLength - InfoLength;
	int M_base = M / Circulant;
	int N_base = N / Circulant;
	int x, y, a, b;
	for (i = 0; i < M_base; i++)
	{
		a = i * Circulant;
		for (j = 0; j < N_base; j++)
		{
			b = j * Circulant;
			if (H_base[i][j] != -1)
			{
				for (k = 0; k < Circulant; k++)
				{
					x = a + k;
					y = (k + H_base[i][j]) % Circulant + b;
					H[x][y] = 1;
				}
			}
		}
	}
}

void matrixGenWimax(
	int CodeLength,
	int InfoLength,
	int Circulant,
	int **H)
{
	double kb;
	kb = (double)InfoLength / (double)Circulant;
	int M_base = 12;
	int N_base = 24;
	int H_base_max[M_base][N_base];
	int K_base;
	K_base = kb;
	if (kb > K_base)
		K_base = kb + 1;

	FILE *base_matrix;
	base_matrix = fopen("dv3R050N1296.txt", "r");

	int i, j, k, l;
	for (i = 0; i < M_base; i++)
	{
		for (j = 0; j < N_base; j++)
		{
			l = fscanf(base_matrix, "%d", &H_base_max[i][j]);
		}
	}
	fclose(base_matrix);
	int M, N;
	N = CodeLength;
	M = CodeLength - InfoLength;
	int x, y, a, b;
	for (i = 0; i < M_base; i++)
	{
		a = i * Circulant;
		for (j = 0; j < N_base; j++)
		{
			b = j * Circulant;
			if (H_base_max[i][j] != -1)
			{
				for (k = 0; k < Circulant; k++)
				{
					x = a + k;
					y = (k + H_base_max[i][j]) % Circulant + b;
					H[x][y] = 1;
				}
			}
		}
	}
}

int GaussianElimination_MRB(int *Perm, int **MatOut, int **Mat, int M, int N)   // 置换记录从外部进来时已经初始化为自然顺序，MatOut是系统的H， Mat是输入的非系统H
{
	int k, n, m, m1, buf, ind, indColumn, nb, *Index, dep, Rank;

	Index = (int *)calloc(N, sizeof(int));    // 注意calloc会清零

	// Triangularization
	indColumn = 0;
	nb = 0;
	dep = 0;
	for (m = 0; m < M; m++)
	{
		if (indColumn == N)
		{
			dep = M - m;        // dep全零行的个数，此时m表示第一个全零行的索引，也表示其上非全零行的个数
			break;
		}

		for (ind = m; ind < M; ind++)
		{
			if (Mat[ind][indColumn] != 0)
				break;
		}
		// If a "1" is found on the column, permutation of rows
		if (ind < M)
		{
			for (n = indColumn; n < N; n++)
			{
				buf = Mat[m][n];
				Mat[m][n] = Mat[ind][n];
				Mat[ind][n] = buf;
			}
			// bottom of the column ==> 0
			for (m1 = m + 1; m1 < M; m1++)
			{
				if (Mat[m1][indColumn] == 1)
				{
					for (n = indColumn; n < N; n++)
						Mat[m1][n] = Mat[m1][n] ^ Mat[m][n];
				}
			}
			Perm[m] = indColumn;
		}
		// else we "mark" the column.
		else
		{
			Index[nb++] = indColumn;
			m--;
		}

		indColumn++;
	}

	Rank = M - dep;

	for (n = 0; n < nb; n++)
		Perm[Rank + n] = Index[n];
	// Permutation of the matrix
	for (m = 0; m < M; m++)
	{
		for (n = 0; n < N; n++)
			MatOut[m][n] = Mat[m][Perm[n]];
	}

	// Diagonalization
	for (m = 0; m < (Rank - 1); m++)
	{
		for (n = m + 1; n < Rank; n++)
		{
			if (MatOut[m][n] == 1)
			{
				for (k = n; k < N; k++)
					MatOut[m][k] = MatOut[n][k] ^ MatOut[m][k];
			}
		}
	}
	free(Index);
	return (Rank);
}


int main()
{
	int CodeLength = 1560;
	int InfoLength = 520;
	int Circulant = 52;
	int **MatH, **MatG, **H;
	int *PermG;
	int rank;
	int i, j, k;
	FILE *fp;
	int *codeword_r, *codeword;
	int M, N;
	M = CodeLength - InfoLength;
	N = CodeLength;
	PermG = (int *)malloc(N * sizeof(int));
	MatH = (int **)malloc(M * sizeof(int *));
	for (i = 0; i < N; i++)
		MatH[i] = (int *)malloc(N * sizeof(int));
	MatG = (int **)malloc(M * sizeof(int *));
	for (i = 0; i < N; i++)
		MatG[i] = (int *)malloc(N * sizeof(int));
	H = (int **)malloc(M * sizeof(int *));
	for (i = 0; i < N; i++)
		H[i] = (int *)malloc(N * sizeof(int));

	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			MatH[i][j] = 0;
			MatG[i][j] = 0;
			H[i][j] = 0;
		}
	}
	for (i = 0; i < N; i++)
		PermG[i] = i;
	matrixGenBG2(CodeLength, InfoLength, Circulant, MatH);
	for (i = 0; i < M; i++)
	{
		for (j = 0; j < N; j++)
		{
			H[i][j] = MatH[i][j];
		}
	}

	/*	fp = fopen("H_BG2K520Z52N2600","w");
        fprintf(fp,"./make-pchk H_BG2K520Z52N2600.h %d %d ",M,N);
        for(i=0;i<M;i++){
                for(j=0;j<N;j++){
                        if(MatH[i][j] == 1) fprintf(fp,"%d:%d ",i,j);
                }
        }
        fclose(fp);
*/
	rank = GaussianElimination_MRB(PermG, MatG, MatH, M, N);     // 输入的MatH是非系统形式的H，这个函数对MatH本身进行行变换，MatH出来后接近于系统；MatG是系统的H，它是接近系统形式的MatH经列变换后得来
	printf("rank = %d\n", rank);

	codeword = (int *)malloc(N * sizeof(int));
	codeword_r = (int *)malloc(N * sizeof(int));
	for (i = 0; i < rank; i++)
		codeword[i] = 0;
	for (i = rank; i < N; i++)
		codeword[i] = floor(drand48() * 2);
	for (i = rank - 1; i >= 0; i--)
	{
		for (j = rank; j < N; j++)
		{
			codeword[i] = codeword[i] ^ (MatG[i][j] * codeword[j]);
		}
	}
	for (j = 0; j < N; j++)
		codeword_r[PermG[j]] = codeword[j];    // 反列变换？
	for (i = 0; i < M; i++)
	{
		k = 0;
		for (j = 0; j < N; j++)
		{
			k = k + codeword_r[j] * MatH[i][j];
		}
		k = k % 2;
		printf("%d", k);
	}

	fp = fopen("BG2_a13_Z52_K520_R033_DV_index.txt", "w");
	for (j = 0; j < N; j++)
	{
		fprintf(fp, "%d:", j);
		for (i = 0; i < M; i++)
		{
			if (H[i][j] == 1)
				fprintf(fp, " %d", i);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	fp = fopen("BG2_a13_Z52_K520_R033_DV_degree.txt", "w");
	for (j = 0; j < N; j++)
	{
		fprintf(fp, "%d:", j);
		k = 0;
		for (i = 0; i < M; i++)
		{
			if (H[i][j] == 1)
				k = k + 1;
		}
		fprintf(fp, "%d\n", k);
	}
	fclose(fp);

	fp = fopen("BG2_a13_Z52_K520_R033_DC_index.txt", "w");
	for (i = 0; i < M; i++)
	{
		fprintf(fp, "%d:", i);
		for (j = 0; j < N; j++)
		{
			if (H[i][j] == 1)
				fprintf(fp, " %d", j);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	fp = fopen("BG2_a13_Z52_K520_R033_DC_degree.txt", "w");
	for (i = 0; i < M; i++)
	{
		fprintf(fp, "%d:", i);
		k = 0;
		for (j = 0; j < N; j++)
		{
			if (H[i][j] == 1)
				k = k + 1;
		}
		fprintf(fp, "%d\n", k);
	}
	fclose(fp);

	/*	FILE *code_fp;
	code_fp = fopen("codeword_BG2K520Z52N2600.txt","w");
	for (i=0;i<N;i++)	fprintf(code_fp,"%d\n",codeword_r[i]);
	fclose(code_fp); */
	return 0;
}
