/*
 *  prodcons module
 *  Producer Consumer module
 *
 *  Implements routines for the producer consumer module based on
 *  chapter 30, section 2 of Operating Systems: Three Easy Pieces
 *
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 */

// Include only libraries for this module
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "counter.h"
#include "matrix.h"
#include "pcmatrix.h"
#include "prodcons.h"

//Buffer mutex lock protects adding and removing data from the shared bounded buffer
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
//Mutex lock combines with condition variables to signal when the bounded buffer is full or empty
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Condition variables here
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

// Producer consumer data structures
counter_t prod;
counter_t cons;

// Bounded buffer bigmatrix defined in prodcons.h
// Matrix ** bigmatrix;
// Assuming we have an array bigmatrix with producer index and consumer index
// bigmatrix = 1 2 3 4 5 6 7 8 9 10 11  ....                    ....
//                                      consumer                producer

//Task 2. Implements get() and put() routines for the bounded buffer
 /*
  * Method put the matrix value at producer index
  *   Argument: Matrix value
  *   Return 0: put a matrix to bigmatrix failed
  *          1: put a matrix to bigmatrix successful
  */
int put(Matrix * value)
{
  int check = 0;
  //lock adding data to the shared bounded buffer
  pthread_mutex_lock(&buffer_mutex);
  //If the range between producer index and consumer index < BOUNDED_BUFFER_SIZE
  if((get_cnt(&prod) - get_cnt(&cons)) < BOUNDED_BUFFER_SIZE) {
    //put the matrix value to get_cnt(&prod) % BOUNDED_BUFFER_SIZE position in bigmatrix
    bigmatrix[get_cnt(&prod) % BOUNDED_BUFFER_SIZE] = value;
    //increase producer counter
    increment_cnt(&prod);
    check = 1;
  }
  //unlock adding data to the shared bounded buffer
  pthread_mutex_unlock(&buffer_mutex);
  return check;
}

 /*
  * Method get a matrix at consumer index
  *   Return  NULL get a matrix at consumer index failed
  *           A matrix at consumer index
  */
Matrix * get()
{
  Matrix * mat = NULL;
  //lock removing data from the shared bounded buffer
  pthread_mutex_lock(&buffer_mutex);
  //If the range between producer index and consumer index > 0
  if((get_cnt(&prod) - get_cnt(&cons)) > 0){
    //get a matrix at get_cnt(&cons) % BOUNDED_BUFFER_SIZE position in bigmatrix
    mat = bigmatrix[get_cnt(&cons) % BOUNDED_BUFFER_SIZE];
    //increasing consumer index
    increment_cnt(&cons);
  }
  //unlock removing data from the shared bounded buffer
  pthread_mutex_unlock(&buffer_mutex);
  return mat;
}

//Task 3. Call put() from within prod_worker
/*
 *  Method prod_worker produce NUMBER_OF_MATRICES and place them to the bigmatrix
 *
 *  Algorithm for prod_worker method
 *    - while number of produced matrices < NUMBER_OF_MATRICES do
 *          we have these situations
 *            1. the range between producer index and consumer index == BOUNDED_BUFFER_SIZE
                  1.a. if the produced matrices is equal NUMBER_OF_MATRICES then sending the full signal to cons_worker thread
 *                1.b. else waiting for cons_worker thread consume more matrices
 *            2. the range between producer index and consumer index < BOUNDED_BUFFER_SIZE then generate a new matrix
 *                2.a. if the produced matrices is less than NUMBER_OF_MATRICES then put it to the bigmatrix
 *                2.b. else free a matrix
 */
void *prod_worker(void *arg)
{
  //Cast arg to producer statistic object
  ProdConsStats * prodStats = (ProdConsStats *) arg;
  //while the number of produced matrices < NUMBER_OF_MATRICES
  while(get_cnt(&prod) < NUMBER_OF_MATRICES) {
    //lock the producer worker thread
    pthread_mutex_lock(&mutex);
    //while the range between producer index and consumer index == BOUNDED_BUFFER_SIZE do
    while ((get_cnt(&prod) - get_cnt(&cons)) == BOUNDED_BUFFER_SIZE) {
      //if the number of produced matrices is equal NUMBER_OF_MATRICES
      if(get_cnt(&prod) == NUMBER_OF_MATRICES) {
        //send the full signal to cons_worker thread
        pthread_cond_signal(&full);
        //unlock the prod_worker thread
        pthread_mutex_unlock(&mutex);
        return 0;
      }
      //wake up all waiting threads
      pthread_cond_broadcast(&full);
      //wait for cons_worker thread consume matrices
      pthread_cond_wait(&empty, &mutex);
    }
    //generate a new matrix
    Matrix * mat = GenMatrixRandom();
    //if the produced matrices is less than NUMBER_OF_MATRICES and put it to the bigmatrix successful
    if(get_cnt(&prod) < NUMBER_OF_MATRICES && put(mat)) {
      //compute the producer statistic
      prodStats->sumtotal += SumMatrix(mat);
      prodStats->matrixtotal += 1;
    } else {
      //Free matrix
      FreeMatrix(mat);
    }
    //send the signal full to cons_worker thread
    pthread_cond_signal(&full);
    //unlock the producer worker thread
    pthread_mutex_unlock(&mutex);
  }
  //wake up all waiting threads
  pthread_cond_broadcast(&empty);
  return 0;
}

//Task 4: Call get() from within prod_worker
/*
 * Method cons_worker removing matrices from bigmatrix and compute multiplication
 *      1. Remove mat1 from the shared bounded buffer
 *      2. Remove mat2 from the shared bounded buffer
 *      3. Check mat3 = MatrixMultiply(mat1, mat2) if mat3
 *          - NULL return 2.
 *          - is not NULL return 1.
 *      Until
 *         the consumed matrices == NUMBER_OF_MATRICES
 *
 * Algorithm for cons_worker method
 *      while number of consumed matrices < NUMBER_OF_MATRICES do
 *          we have these situations
 *            1. the consumed matrices == the produced matrices
 *                  if the consumed matrices == NUMBER_OF_MATRICES then sending the full signal to prod_worker thread
 *                  else the consumed matrices < NUMBER_OF_MATRICES then waiting for prod_worker thread producing more matrices
 *            2. the produced matrices is greater than the consumed matrices
 *                  2.1. remove mat1 from the bigmatrix. When remove mat1 from the bigmatrix, the counter of consumer will increasing
 *                  2.2. so before removing mat2 we have to check if mat2 in the range of the bigmatrix
 *                     if mat2 is not in the range of the bigmatrix then
 *                          send a full signal to the prod_worker thread
 *                     else
 *                          remove mat2 from the shared bounded buffer
 *                          compute mat3 = MatrixMultiply(mat1,mat2)
 *                              if mat3 is NULL then return 2.2
 *                              else then return 2.1
 */
void *cons_worker(void *arg)
{
  //Cast arg to consumer statistic object
  ProdConsStats * consStats = (ProdConsStats *) arg;
  //while number of consumed matrices < NUMBER_OF_MATRICES
  while(get_cnt(&cons) < NUMBER_OF_MATRICES) {
    //lock consumer worker thread
    pthread_mutex_lock(&mutex);
    //While the consumed matrices == the produced matrices
    while((get_cnt(&prod) - get_cnt(&cons)) == 0) {
      //if the consumed matrices == NUMBER_OF_MATRICES
      if(get_cnt(&cons) == NUMBER_OF_MATRICES) {
        //send the full signal to prod_worker thread
        pthread_cond_signal(&full);
        //unlock consumer worker thread
        pthread_mutex_unlock(&mutex);
        return 0;
      }
      //wake up all waiting threads
      pthread_cond_broadcast(&empty);
      //wait for prod_worker thread producing more matrices
      pthread_cond_wait(&full, &mutex);
    }
    //Remove mat1 from the bigmatrix
    Matrix * mat1 = get();
    //compute the consumer statistic
    consStats->sumtotal += SumMatrix(mat1);
    consStats->matrixtotal += 1;

    //Initialze mat2
    Matrix * mat2 = NULL;
    //Initialze mat3
    Matrix * mat3 = NULL;
    //Repeat until mat3 is not null
    while(mat3 == NULL) {
      //While the consumed matrices == the produced matrices
      while((get_cnt(&prod) - get_cnt(&cons)) == 0) {
        //check if mat2 in the range of the bigmatrix
        if(get_cnt(&cons) == NUMBER_OF_MATRICES) {
          //send the full signal to prod_worker thread
          pthread_cond_signal(&full);
          //unlock consumer worker thread
          pthread_mutex_unlock(&mutex);
          return 0;
        }
        //wake up all waiting threads
        pthread_cond_broadcast(&empty);
        //wait for prod_worker thread producing more matrices
        pthread_cond_wait(&full, &mutex);
      }

      //Remove mat2 from the bigmatrix
      mat2 = get();
      //compute the consumer statistic
      consStats->sumtotal += SumMatrix(mat2);
      consStats->matrixtotal += 1;
      //compute mat3
      mat3 = MatrixMultiply(mat1, mat2);
      //if mat3 is NULL, continue looping
      if(mat3 == NULL) {
        //free mat2
        FreeMatrix(mat2);
      } else { //diplay mat1 x mat2 = mat3
        DisplayMatrix(mat1, stdout);
        printf("     X\n");
        DisplayMatrix(mat2, stdout);
        printf("     =\n");
        DisplayMatrix(mat3, stdout);
        printf("\n");
        consStats->multtotal += 1;
      }
    }

    //free mat1, mat2, mat3
    FreeMatrix(mat1);
    FreeMatrix(mat2);
    FreeMatrix(mat3);
    pthread_cond_signal(&empty);
    //unlock consumer worker thread
    pthread_mutex_unlock(&mutex);
  }
  //wake up all waiting threads
  pthread_cond_broadcast(&full);
  return 0;
}

/*
 * Method initialize the statistics
 *   Return  an object ProdConsStats containing sumtotal, multtotal, matrixtotal
 */
ProdConsStats *init() {
  ProdConsStats *stats = (ProdConsStats *) malloc(sizeof(ProdConsStats));
  if(stats == NULL) return NULL;
  stats -> sumtotal = 0;
  stats -> multtotal = 0;
  stats -> matrixtotal = 0;
  return stats;
}
