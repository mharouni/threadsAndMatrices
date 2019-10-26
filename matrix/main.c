
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



typedef struct {
	int itC;
	int itR;
	int rA;
	int rB;
	int cA;
	int cB;
}input;




bool readMatrix(void);
void * elementCalc(void * args);
void * rowCalc(void * args);
FILE * fp = NULL ;
int * ma = NULL;
int *mb = NULL;
int * mo = NULL;
pthread_mutex_t lock;
int rowA;
int rowB;
int colA;
int colB;


int main(int argc, const char * argv[]) {
	

	int i;
	int j;
	int iterC=0;
	int iterR=0;
	int threadCounter=0;
	int k;
	clock_t time, time2;

	
	
	

	if (readMatrix())
	{
		mo = malloc(rowA * colB * sizeof(int));
		time = clock();
		pthread_t t[rowA *colB];
			for (i=0; i< rowA; i++)
			{
				for (j=0; j<colB; j++)
				{
					pthread_mutex_lock(&lock);
					input * input = malloc(sizeof(input));
					input->cA =colA;
					input->cB = colB;
					input-> rA = rowA;
					input->rB = rowB;
					input-> itR = iterR;
					input-> itC = iterC;
					pthread_mutex_unlock(&lock);
					pthread_create(&t[threadCounter],NULL,elementCalc, input);
					pthread_mutex_lock(&lock);
					iterC++;
					pthread_mutex_unlock(&lock);
					threadCounter++;
					
				}
				pthread_mutex_lock(&lock);
				iterC = 0;
				iterR++;
				pthread_mutex_unlock(&lock);
			}
		
		
			for(k=0;k<threadCounter; k++)
			{
				pthread_join(t[k], NULL);
			}
		time= clock() -time;
		printf("time taken: %f ms \n", (float)time);
	
		for (i=0; i<rowA; i++)
		{
			for (j=0; j<colB; j++)
			{
				printf("%d\t", *(mo + (i * colB ) + j ));
			}
			printf("\n");
		}
		
		
		
		
		
		
		iterC = 0;
		iterR = 0;
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
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
		int i,sum = 0;
		
	
		for(i = 0; i<ca; i++)
		{
		//	printf("\n%d\t%d\n", *(ma + ((r * ca) + i)),*(mb + ((cb * i ) + c)));
			sum+= *(ma + ((r * ca) + i)) * *(mb + ((cb * i ) + c));
			
		}
	//	pthread_mutex_lock(&lock);
		*(mo + ( r * ca) + c) = sum;
		//pthread_mutex_unlock(&lock);
		//printf("%d\n",sum);
	}
	
	
	
	return NULL;
}






void * rowCalc(void * args)
{
	if (mo)
	{
		input * in = (input*) args;
		int c = in->itC;
		int r = in->itR;
		int cb = in->cB;
		int ra = in->rA;
		int ca = in -> cA;
		int i,sum = 0;
		
		
		
		
	}
		return NULL;
}
