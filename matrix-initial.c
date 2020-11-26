#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
int r = 2;
int c = 2;

int main(int argc, char * argv[]) {
	int ** matrix2D;
	int i,j;
	
	//AllocMatrix
	matrix2D = (int**) malloc(sizeof(int *) * r);
	assert(matrix2D != 0);
	for(i = 0; i < r; i++) {
		matrix2D[i] = (int *) malloc(c * sizeof(int));
		assert(matrix2D[i] != 0);
	}
	
	//Initial Matrix
	for(i = 0; i < r; i++){
		for(j = 0; j < c; j++){
			int * temp = matrix2D[i];
			printf("Matrix[%d][%d]:",i,j);
			scanf("%d", &temp[j]);			
		}
	}
	
	
	//Print Matrix
	for(i = 0; i < r; i++){
		for(j = 0; j < c; j++){
			int * mm = matrix2D[i];
			printf("matrix[%d][%d]=%d \n",i,j,mm[j]);			
		}
	}
	
	//Find average
	int totalSum = 0;
	int element = 0;
	for(i = 0; i < r; i++){
		for(j = 0; j < c; j++) {
			int *matrix = matrix2D[i];
			totalSum = totalSum + matrix[j];
			element = element + 1;
		}
	}
	
	printf("AVG ELEMENT matrix-total-sum=%d element-count=%d avg-elem=%d\n",totalSum, element, totalSum / element);
	
	

	for (i=0; i<r; i++)
	{
		free(matrix2D[i]);
	}
	free(matrix2D);

	return 0;
}
