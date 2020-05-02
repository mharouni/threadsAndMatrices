
//
//  main.c
//  matrix
//
//  Created by Mohamed Harouni on 10/22/19.
//  Copyright © 2019 Mohamed Harouni. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <strings.h>
#include <stdbool.h>
#include <time.h>



typedef struct {// struct use the pass arguments to the function called in the thread.
	int itC;// col iterator
	int itR;// row iterator
	int rA;//number of rows in A
	int rB;//rows if B
	int cA;//cols of A
	int cB;// cols of B
	int index;// offset from the result vector pointer.
}input;




bool readMatrix(void);//reads the input file and populates the dedicated memory
void * elementCalc(void * args);//method 1
void * rowCalc(void * args); //method 2
FILE * fp = NULL ;// file poitner
int * ma = NULL;// matrix A
int *mb = NULL;// matrix B
int * mo = NULL;//element per thread result matrix
int * mo2 = NULL;// row per thread result matrix
// ma * mb is the order of matrix multiplication
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;// mutex lock init
int rowA;//number of rows of matrix A
int rowB; // rows of matrix B
int colA;//cols of A
int colB;// cols of B



int main(int argc, const char * argv[]) {


	int i;
	int j;
	int iterC=0;//cols iterator
	int iterR=0;// rows iterator
	int threadCounter=0;
	int k;
	int ind =0;// offset from the result vector pointer.
	clock_t time, time2;// variables to calculate clock cycles it took to complete each method





	if (readMatrix())
	{
		mo = malloc(rowA * colB * sizeof(int));
		mo2 = malloc(rowA * colB * sizeof(int));
		time = clock();
		pthread_t t[rowA *colB];
		for (i=0; i< rowA; i++)
		{
			for (j=0; j<colB; j++)
			{
					pthread_mutex_lock(&lock);//lock the shared vaiables while they're being updated.(cautionary step)
					input * input2 = malloc(sizeof(input));
					input2->cA =colA;
					input2->cB = colB;
					input2-> rA = rowA;
					input2->rB = rowB;
					input2-> itR = iterR;
					input2-> itC = iterC;
					input2-> index = ind;
					pthread_mutex_unlock(&lock);
					pthread_create(&t[threadCounter],NULL,elementCalc, input2);
					pthread_mutex_lock(&lock);
					ind++;
					iterC++;//increments the column to be multiplied
					threadCounter++;
					pthread_mutex_unlock(&lock);

				}
				pthread_mutex_lock(&lock);//lock the shared vaiables while they're being updated.
				iterC = 0;// resets col counter
				iterR++;//increment rows to be multiplied
				pthread_mutex_unlock(&lock);
			}


			for(k=0;k<threadCounter; k++)
			{
				pthread_join(t[k], NULL);
			}

		time= clock() -time;
		printf("time taken: %f ms \n", (float)time);


		;


		pthread_mutex_lock(&lock);// reset the variables that are used by both methods
		iterC = 0;
		iterR = 0;
		threadCounter = 0;
		ind=0;
		pthread_mutex_unlock(&lock);
		pthread_t t2[rowA];


		time2 = clock();
		for(k=0; k < rowA; k++)
		{
			pthread_mutex_lock(&lock);
			input * input1 = malloc(sizeof(input));
			input1->cA =colA;
			input1->cB = colB;
			input1-> rA = rowA;
			input1->rB = rowB;
			input1-> itR = iterR;
			input1-> itC = iterC;
			input1-> index = ind;
			pthread_mutex_unlock(&lock);
			pthread_create(&t2[threadCounter], NULL, rowCalc, input1);
			threadCounter ++;
			pthread_mutex_lock(&lock);
			iterR++;
			ind+=colB;// adds the number of elemtes in  a single row to the offset so it points to the first element of the following row.
			pthread_mutex_unlock(&lock);

		}
		for(k=0;k<threadCounter; k++)
		{
			pthread_join(t2[k], NULL);
		}
		time2 = clock() - time2;
		printf("time taken: %f ms \n", (float)time2);


		pthread_mutex_lock(&lock);
		fp = fopen("output.txt", "w");
		for (i=0; i<rowA; i++)
		{
			for (j=0; j<colB; j++)
			{
				printf("%d\t", *(mo + (i * colB ) + j ));
				fprintf(fp, "%d\t",*(mo + (i * colB ) + j ));
			}
			fprintf(fp, "\n");
			printf("\n");
		}
		fprintf(fp, "END1\tCPU time : %lf seconds\n\n",(double)time/CLOCKS_PER_SEC);
		printf("\n");

		for (i=0; i<rowA; i++)
		{
			for (j=0; j<colB; j++)
			{
				printf("%d\t", *(mo2 + (i * colB  ) + j ));
				fprintf(fp, "%d\t",*(mo2 + (i * colB ) + j ));
			}
			fprintf(fp, "\n");
			printf("\n");
		}
		fprintf(fp, "END2\ttime : %lf seconds\n",(double)time2/CLOCKS_PER_SEC);
		//printf("%lf",139/CLOCKS_PER_SEC);
		fclose(fp);
		pthread_mutex_unlock(&lock);
















	}
	else
		printf("Error, matrices can not be multiplied\n");

	return 0;
}















bool readMatrix()
{

	int i, j;

	if ( (fp = fopen("input.txt", "r")) )

	{
		fscanf(fp, "%d%d", &rowA, &colA);
		ma = malloc( (colA) * (rowA) * sizeof( int ));

		for (i=0; i<rowA; i++)
		{
			for (j=0; j<colA; j++)
			{
				fscanf(fp, "%d", (ma+ (i * (colA) + j)));
				//printf("%d\t",*(ma+ (i * (colA) + j)));
			}
			printf("\n");
		}

		fscanf(fp, "%d%d", &rowB, &colB);
		mb = malloc( (colB) * (rowB) * sizeof( int ));

		for (i=0; i<rowB; i++)
		{
			for (j=0; j<colB; j++)
			{
				fscanf(fp, "%d", (mb+ (i * (colB) + j)));
				//printf("%d\t",*(mb + (i * (colB) + j)));
			}
			printf("\n");
		}
		fclose(fp);
		if (rowB == colA)
			return true;
		else
			return false;
	}
	return true;
}
void* elementCalc(void * args)
{
	if (mo)
	{
		input * in = (input*) args;
		int c = in->itC;
		int r = in->itR;
		int cb = in->cB;
		int ca = in -> cA;
		int innd = in->index;
		int i,sum = 0;

		for(i = 0; i<ca; i++)
		{
			sum+= *(ma + ((r * ca) + i)) * *(mb + ((cb * i ) + c));//multiplies each element of a row by each element of the col to add them to gether and saves the result in the right place in the output matrix
			//printf("%d\n",sum);
		}
		printf("%d\n",sum);
		//*(mo + ( r * ca) + c) = sum;
		*(mo + innd) = sum;
		}



	return NULL;
}






void * rowCalc(void * args)
{
	if (mo2)
	{
		input * in = (input*) args;
		//int c = in->itC;
		int r = in->itR;
		int cb = in->cB;
		//int ra = in->rA;
		int ca = in -> cA;
		int innd = in->index;
		int i, j, sum = 0;
		for (i=0;i<cb; i++)
		{// iterates on each colomn in matrix B and multiplies it by the suitable row in matrix a to output a row in the result matrix
			for (j=0; j< ca; j++) {
				sum += *(ma + ((r * ca) + j)) * *(mb + ((cb * j ) + i));//multiplies each element of a row by each element of the col to add them to gether and saves the result in the right place in the output matrix
			}
			//*(mo2+ (r * ca) +i) = sum;
		//printf("%d\n",sum);
			*(mo2 + innd) = sum;
			sum = 0;
			innd ++;
		}



	}
		return NULL;
}
