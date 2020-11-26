#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
int m1r = 5;
int m1c = 5;
int m2r = 5;
int m2c = 5;

int main(int argc, char * argv[]) {
	int ** matrix2D1;
	int ** matrix2D2;
	int *temp,*m1, *m2;
	int i,j;
	
	//Alloc Matrix 1
	matrix2D1 = (int**) malloc(sizeof(int *) * m1r);
	assert(matrix2D1 != 0);
	for(i = 0; i < m1r; i++) {
		matrix2D1[i] = (int *) malloc(m1c * sizeof(int));
		assert(matrix2D1[i] != 0);
	}
	
	//Alloc Matrix 2
	matrix2D2 = (int**) malloc(sizeof(int *) * m2r);
	assert(matrix2D2 != 0);
	for(i = 0; i < m2r; i++) {
		matrix2D2[i] = (int *) malloc(m2c * sizeof(int));
		assert(matrix2D2[i] != 0);
	}
	
	//Initial Matrix 1
	for(i = 0; i < m1r; i++){
		for(j = 0; j < m1c; j++){
			temp = matrix2D1[i];
			printf("Matrix 1 [%d][%d]:",i,j);
			scanf("%d", &temp[j]);			
		}
	}
	
	//Initial Matrix 2
	for(i = 0; i < m2r; i++){
		for(j = 0; j < m2c; j++){
			temp = matrix2D2[i];
			printf("Matrix 2 [%d][%d]:",i,j);
			scanf("%d", &temp[j]);			
		}
	}
	
	printf("Matrix 1\n");
	//Print Matrix 1
	for(i = 0; i < m1r; i++){
		for(j = 0; j < m1c; j++){
			temp = matrix2D1[i];
			printf("%d ",temp[j]);			
		}
		printf("\n");			
	}
	printf("Matrix 2\n");
	//Print Matrix 2
	for(i = 0; i < m2r; i++){
		for(j = 0; j < m2c; j++){
			temp = matrix2D2[i];
			printf("%d ",temp[j]);			
		}
		printf("\n");			
	}
	printf("Substract of Matrix 1 and Matrix 2:\n");	
	//Sum two matrices
	for(i = 0; i < m2r; i++){
		for(j = 0; j < m2c; j++){
			m1 = matrix2D1[i];
			m2 = matrix2D2[i];
			printf("%d ",m1[j] - m2[j]);			
		}
		printf("\n");			
	}


	for (i=0; i<m1r; i++)
	{
		free(matrix2D1[i]);
	}
	free(matrix2D1);
	
	for (i=0; i<m2r; i++)
	{
		free(matrix2D2[i]);
	}
	free(matrix2D2);

	return 0;
}
