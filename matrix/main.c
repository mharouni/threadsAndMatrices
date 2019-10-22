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
bool readMatrix(FILE * fp, int ** a, int ** b, int * rowA, int * rowB, int * colA, int * colB);
void* elementByElement(void * args);



int main(int argc, const char * argv[]) {
	
	
	FILE * fp = NULL ;
	int * ma = NULL;
	int *mb = NULL;
	int rowA;
	int rowB;
	int colA;
	int colB;
	if (readMatrix(fp, &ma, &mb, &rowA, &rowB, &colA, &colB))
	{

		
	}
	else
		printf("Error, matrices can not be multiplied\n");
	
	return 1;
}



bool readMatrix(FILE * fp, int ** a, int ** b, int * rowA, int * rowB, int * colA, int * colB)
{

	int i, j;
	
	if ( (fp = fopen("input.txt", "r")) )
		
	{
		fscanf(fp, "%d%d", rowA, colA);
		*a = malloc( *(colA) * *(rowA) * sizeof( int ));
		
		for (i=0; i<*rowA; i++)
		{
			for (j=0; j<*colA; j++)
			{
				fscanf(fp, "%d", *(a)+(j+*(colA)*i));
			}
		}
		
		fscanf(fp, "%d%d", rowB, colB);
		b = malloc( *(colB) * *(rowB) * sizeof( int ));
		
		for (i=0; i<*rowB; i++)
		{
			for (j=0; j<*colB; j++)
			{
				fscanf(fp, "%d", *(b)+(j+*(colB)*i));
			}
		}
		
		if (*rowA == *colB)
			return true;
		else
			return false;
	}
	else
	return false;
}




