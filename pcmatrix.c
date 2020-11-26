/*
 *  pcmatrix module
 *  Primary module providing control flow for the pcMatrix program
 *
 *  Producer consumer bounded buffer program to produce random matrices in parallel
 *  and consume them while searching for valid pairs for matrix multiplication.
 *  Matrix multiplication requires the first matrix column count equal the
 *  second matrix row count.
 *
 *  A matrix is consumed from the bounded buffer.  Then matrices are consumed
 *  from the bounded buffer, one at a time, until an eligible matrix for multiplication
 *  is found.
 *
 *  Totals are tracked using the ProdConsStats Struct for:
 *  - the total number of matrices multiplied (multtotal from consumer threads)
 *  - the total number of matrices produced (matrixtotal from producer threads)
 *  - the total number of matrices consumed (matrixtotal from consumer threads)
 *  - the sum of all elements of all matrices produced and consumed (sumtotal from producer and consumer threads)
 *
 *  Correct programs will produce and consume the same number of matrices, and
 *  report the same sum for all matrix elements produced and consumed.
 *
 *  Each thread produces a total sum of the value of
 *  randomly generated elements.  Producer sum and consumer sum must match.
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include "matrix.h"
#include "counter.h"
#include "prodcons.h"
#include "pcmatrix.h"

int main (int argc, char * argv[])
{
  // Process command line arguments
  int numw = NUMWORK;
  if (argc==1)
  {
    BOUNDED_BUFFER_SIZE=MAX;
    NUMBER_OF_MATRICES=LOOPS;
    MATRIX_MODE=DEFAULT_MATRIX_MODE;
    printf("USING DEFAULTS: worker_threads=%d bounded_buffer_size=%d matricies=%d matrix_mode=%d\n",numw,BOUNDED_BUFFER_SIZE,NUMBER_OF_MATRICES,MATRIX_MODE);
  }
  else
  {
    if (argc==2)
    {
      numw=atoi(argv[1]);
      BOUNDED_BUFFER_SIZE=MAX;
      NUMBER_OF_MATRICES=LOOPS;
      MATRIX_MODE=DEFAULT_MATRIX_MODE;
    }
    if (argc==3)
    {
      numw=atoi(argv[1]);
      BOUNDED_BUFFER_SIZE=atoi(argv[2]);
      NUMBER_OF_MATRICES=LOOPS;
      MATRIX_MODE=DEFAULT_MATRIX_MODE;
    }
    if (argc==4)
    {
      numw=atoi(argv[1]);
      BOUNDED_BUFFER_SIZE=atoi(argv[2]);
      NUMBER_OF_MATRICES=atoi(argv[3]);
      MATRIX_MODE=DEFAULT_MATRIX_MODE;
    }
    if (argc==5)
    {
      numw=atoi(argv[1]);
      BOUNDED_BUFFER_SIZE=atoi(argv[2]);
      NUMBER_OF_MATRICES=atoi(argv[3]);
      MATRIX_MODE=atoi(argv[4]);
    }
    printf("USING: worker_threads=%d bounded_buffer_size=%d matricies=%d matrix_mode=%d\n",numw,BOUNDED_BUFFER_SIZE,NUMBER_OF_MATRICES,MATRIX_MODE);
  }

  int i;
  time_t t;
  // Seed the random number generator with the system time
  srand((unsigned) time(&t));

  //Task 1. Implements a bounded buffer
  bigmatrix = (Matrix **) malloc(sizeof(Matrix *) * BOUNDED_BUFFER_SIZE);

  printf("Producing %d matrices in mode %d.\n",NUMBER_OF_MATRICES,MATRIX_MODE);
  printf("Using a shared buffer of size=%d\n", BOUNDED_BUFFER_SIZE);
  printf("With %d producer and consumer thread(s).\n",numw);
  printf("\n");

  //Task 6. use an array of producer threads, and an array of consumer threads
  pthread_t pr[numw];
  pthread_t co[numw];

  //Task 5. create a produce thread and a consume thread
  // pthread_t pr;
  // pthread_t co;

  // consume ProdConsStats from producer and consumer threads
  ProdConsStats * prodStats = init();
  ProdConsStats * consStats = init();

  // pthread_create(&pr, NULL, prod_worker, prodStats);
  // pthread_create(&co, NULL, cons_worker, consStats);
  for (i = 0; i < numw; i++) {
    pthread_create(&pr[i], NULL, prod_worker, prodStats);
    pthread_create(&co[i], NULL, cons_worker, consStats);
  }

  // pthread_join(pr, NULL);
  // pthread_join(co, NULL);
  for(i = 0; i < numw; i++) {
    pthread_join(pr[i], NULL);
    pthread_join(co[i], NULL);
  }


  // add up total matrix stats in prs, cos, prodtot, constot, consmul
  printf("Sum of Matrix elements --> Produced=%d = Consumed=%d\n",prodStats->sumtotal,consStats->sumtotal);
  printf("Matrices produced=%d consumed=%d multiplied=%d\n",prodStats->matrixtotal,consStats->matrixtotal,consStats->multtotal);

  return 0;
}
