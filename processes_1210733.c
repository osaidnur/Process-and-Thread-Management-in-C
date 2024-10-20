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

// Note : this number of precesses without the parent , so the actual number is increamented by 1
#define process_num 3
#define array_size 100

// define the portion for each process , so that the array rows will be divided equally between all processes
int process_portion = ceil((double)array_size / (process_num + 1));

// Function Prototype
void fillArray(long long elem, int n, int arr[n][n]);
void multiplyMatrices(int begin, int end, int arr1[][array_size], int arr2[][array_size], int res[][array_size]);
void updateRes(int begin, int end, int arr[][array_size], int res[][array_size]);

int main()
{
    // construct the arrays we need for our programe
    int A[array_size][array_size] = {}, B[array_size][array_size] = {}, result[array_size][array_size] = {};

    // Fill both arrays(A and B) as described in the task file
    // My ID number is 1210733 , and My birth date is 2003
    // so ID*Birth date = 2425098199
    fillArray(1210733, array_size, A);
    fillArray(2425098199, array_size, B);

    // the struct timeeval exists in the libraray sys/time.h
    struct timeval start, end;

    // The idea of the next for loop is to store the indeces of ranges
    // that our processes must apply the matrix multiplication to
    // For example, if we have 2 processes and a 100 array size, the array "ranges"
    // contains {0,50,100} , so that the child can access these indeces easily
    int ranges[process_num + 2] = {};
    int cur = 0;
    for (int k = 0; k < process_num + 2; k++)
    {
        ranges[k] = cur;
        cur += process_portion;
    }

    // the array 'pids' contains the processes id's which can be either parent or childs
    // the array 'pipes' represents the connection way between childs
    int pids[process_num] = {};
    int pipes[process_num + 1][2] = {};

    // intializing the pipes
    for (int i = 0; i < process_num + 1; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            printf("Error with creating pipe\n");
            return 1;
        }
    }

    // start Recording the time
    gettimeofday(&start, NULL);

    // Creating the childs processes using fork() , i represents the process number
    for (int i = 0; i < process_num; i++)
    {
        // store the process id's in the array
        pids[i] = fork();
        if (pids[i] == -1)
        {
            printf("Error with creating process\n");
            return 2;
        }
        // Child process
        if (pids[i] == 0)
        {
            // we want to close all pipes that we don't need it , the result of this loops
            // that we will let one open pipe for writing 
            for (int j = 0; j < process_num + 1; j++)
            {
                close(pipes[j][0]);
                if (i + 1 != j)
                {
                    close(pipes[j][1]);
                }
            }

            // perform the multiply operation
            multiplyMatrices(ranges[i], ranges[i + 1], A, B, result);

            //********************************************** FOR TESTING
            // printf("\nThis is Child %d : \n", i);
            // for (int i = 0; i < array_size; i++)
            // {
            //     for (int j = 0; j < array_size; j++)
            //     {
            //         printf("%d ", result[i][j]);
            //     }
            //     printf("\n");
            // }
            //********************************************** FOR TESTING

            // pass the array to the pipe(to let the parent read it) using writing side
            if (write(pipes[i + 1][1], result, sizeof(result)) == -1)
            {
                printf("Error at writing\n");
                return 4;
            }

            // close the write side of the pipe when we finish writing
            close(pipes[i + 1][1]);

            // terminates the child process and return to the main process
            return 0;
        }
    }

    // When the previous loop terminates , all child processes are finished so we return to the
    // Main process .

    // the parent process will perform the multiply operation for the
    // last range of the array , after this step , we will have only the last
    // part of the final result array .

    // perform the multiply operation for the last time by the parent
    multiplyMatrices(ranges[process_num], ranges[process_num + 1], A, B, result);

    // The parent must wait all children to perform the matrix multiplication on
    // the array result and return it to the parent , this loop that tells the parent
    // to wait for all children
    for (int i = 0; i < process_num; i++)
    {
        wait(NULL);
    }

    // in the following lines, we will recive each part of the multiplies array 
    // from the children pipes , remember that we are sured here than all children 
    // finished their work and terminated ,and the only remaining process is the main process
    
    // temporary array to recive the array from the child 
    int tempRes[array_size][array_size] = {};
    
    // in this loop , we will iterate over all read sides in the pipes to get each part of 
    // the multiplied array (each recived array will have just a specific range of rows multipled
    // and the other rows are zeros ) , and our point is to combine them in the final result array
    for (int i = 1; i < process_num + 1; i++)
    {
        // close the write side of the pipe
        close(pipes[i][1]);
        //read the array from the pipe for each child
        read(pipes[i][0], tempRes, sizeof(tempRes));
        // update our result array (final array) with the nonzero rows of the tempRes
        // to do this , we pass two parameters to select the begin and the end of our update function
        updateRes(ranges[i - 1], ranges[i], tempRes, result);
        // close the read side of the pipe 
        close(pipes[i][0]);
    }
    
    //************************************************* FOR TESTING
    // printf("This is the final matrix  !\n");
    // for (int i = 0; i < array_size; i++)
    // {
    //     for (int j = 0; j < array_size; j++)
    //     {
    //         printf("%d ", result[i][j]);
    //     }
    //     printf("\n");
    // }
    //************************************************* FOR TESTING

    // End Recording the time
    gettimeofday(&end, NULL);

    // Calculating the execution time (note: this calculations are defined in the library previously)
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;

    printf("The execution time for multiple processes way = %f second when using %d processes\n", elapsed, process_num + 1);
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

// updates the final result array with the nonzero elements from the array arr
void updateRes(int begin, int end, int arr[][array_size], int res[][array_size])
{
    for (int i = begin; i < end; i++)
    {
        for (int j = 0; j < array_size; j++)
        {
                res[i][j] = arr[i][j];
        }
    }
}