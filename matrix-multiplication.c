#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
int m1r = 3;
int m1c = 4;
int m2r = 4;
int m2c = 2;

int main(int argc, char * argv[]) {
	int ** matrix2D1, ** matrix2D2, ** matrix2D ;
	int *temp,*m1, *m2, *mm;
	int i,j,k;
	
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
	
	//Alloc Multiplication Matrix
	matrix2D = (int**) malloc(sizeof(int *) * m1r);
	assert(matrix2D != 0);
	for(i = 0; i < m1r; i++) {
		matrix2D[i] = (int *) malloc(m2c * sizeof(int));
		assert(matrix2D[i] != 0);
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
	
	//Initial Multiplication Matrix
	for(i = 0; i < m1r; i++){
		for(j = 0; j < m2c; j++){
			temp = matrix2D[i];
			temp[j] = 0;			
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
	printf("Multiplication of Matrix 1 and Matrix 2:\n");	
	//Sum two matrices
	for(i=0; i < m1r; i++){
		mm = matrix2D[i];	
		m1 = matrix2D1[i];
		for(j = 0; j < m2c; j++){
			for(k = 0; k < m1c; k++){
				m2 = matrix2D2[k];		
				mm[j] += m1[k]*m2[j];			
			}
		}	
	}
	
	//Print Multiplication Matrix 2
	for(i = 0; i < m1r; i++){
		for(j = 0; j < m2c; j++){
			temp = matrix2D[i];
			printf("%d ",temp[j]);			
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
	
	for (i=0; i<m1r; i++)
	{
		free(matrix2D[i]);
	}
	free(matrix2D);

	return 0;
}
