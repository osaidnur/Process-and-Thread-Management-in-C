/*
Name    : Osaid Hasan Nur
ID      : 1210733
Section : 4
Dr Bashar Tahayna
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define thread_num 4
#define array_size 100

// define the portion for each thread , so that the array rows will be divided equally between all threads
int thread_portion = ceil((double)array_size / thread_num);

// Function Prototype
void fillArray(long long elem, int n, int arr[n][n]);
void multiplyMatrices(int begin, int end, int arr1[][array_size], int arr2[][array_size], int res[][array_size]);
void *threadFunc(void *arg);

// construct the arrays we need for our programe , we make them Global so that we
// can access them from all threads and deal with them easily
int A[array_size][array_size], B[array_size][array_size], result[array_size][array_size];

// This the thread function
void *threadFunc(void *arg)
{
    // the parameter for this function is sent from the main thread  , and it's 
    // a pointer to int that represents the actual number of thread
    int progress = *((int *)arg);

    // Perform the multiply operation by the thread
    multiplyMatrices(progress * thread_portion, (progress + 1) * thread_portion, A, B, result);

    // ****************************************************** FOR TESTING
    // printf("\nThe progress is : %d \n" , progress);
    // for (int i = 0; i < array_size; i++)
    // {
    //     for (int j = 0; j < array_size; j++)
    //     {
    //         printf("%d ",result[i][j]) ;
    //     }
    //     printf("\n");
    // }
    // printf("\n") ;
    // ****************************************************** FOR TESTING

    // free the pointer and send it to the garbage
    free(arg);
}

int main()
{
    // Fill both arrays(A and B) as described in the task file
    // My ID number is 1210733 , and My birth date is 2003
    // so ID*Birth date = 242509819
    fillArray(1210733, array_size, A);
    fillArray(2425098199, array_size, B);

    // the following array stores informations about created threads
    pthread_t th[thread_num];

    // the struct timeeval exists in the libraray sys/time.h
    struct timeval start, end;
    
    // start Recording the time
    gettimeofday(&start, NULL);

    // start generating threads
    for (int i = 0; i < thread_num; i++)
    {    
        // allocating memory for a pointer that we will send it as an argument to thread function
        int *prog = malloc(sizeof(int));
        
        // Error checking
        if (prog == NULL)
            printf("Error in allocating memory !\n");
        
        // assign the thread number 'i' to the value of pointer , this value will be passed to 
        // thread function , and the function will depend on it to select the appropriate range 
        // from array and let the thread to perform multiplication 
        *prog = i;
        
        // create the thread , call the thread function , and pass the argument (i)
        if (pthread_create(th + i, NULL, &threadFunc, prog) != 0)
        {
            perror("Failed to create threads ! \n");
            return 1;
        }
    }

    // Joining the thread to the main thread and wait for all threads to finish their work
    for (int i = 0; i < thread_num; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            return 2;
        }
    }

    // Recording the End time of execution
    gettimeofday(&end, NULL);

    // Calculating the execution time (note: this calculations are defined in the library previously)
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;

    printf("The execution time for multiple joinable threads way = %f second when using %d threads\n", elapsed,thread_num);

    return 0;
}

// the following fill function takes an element which is the number that
// we will repeat it to generate our array
void fillArray(long long elem, int n, int arr[n][n])
{
    long long temp = elem;
    // the integer count represents the number of digits for the element,
    // in out task it's either 7 (if it's the University ID) or 10
    // (if it's ID*Birth Date)
    int count = 0;
    while (temp != 0)
    {
        temp /= 10;
        count++;
    }
    // this array to store the numbers that we are going the array with
    int rep[count];
    for (int i = 0; i < count; i++)
    {
        rep[count - 1 - i] = elem % 10;
        elem /= 10;
    }
    int ptr = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            arr[i][j] = rep[ptr % count];
            ptr++;
        }
    }
}

// This function multiplyes two arrays for a specific range
void multiplyMatrices(int begin, int end, int arr1[array_size][array_size], int arr2[array_size][array_size], int res[array_size][array_size])
{
    if (end > array_size)
        end = array_size;
    for (int row = begin; row < end; row++)
    {
        for (int column = 0; column < array_size; column++)
        {
            int tempRes = 0;
            for (int elem = 0; elem < array_size; elem++)
            {
                tempRes += (arr1[row][elem] * arr2[elem][column]);
            }
            res[row][column] = tempRes;
        }
    }
}
