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

#define array_size 100
// Function Prototype
void fillArray(long long elem, int n, int arr[n][n]);
void multiplyMatrices(int begin, int end, int arr1[][array_size], int arr2[][array_size], int res[][array_size]);

int main()
{
    // construct the arrays we need for our programe
    int A[array_size][array_size], B[array_size][array_size], result[array_size][array_size];

    // Fill both arrays(A and B) as described in the task file
    // My ID number is 1210733 , and My birth date is 2003
    // so ID*Birth date = 2425098199
    fillArray(1210733, array_size, A);
    fillArray(2425098199, array_size, B);

    // the struct timeeval exists in the libraray sys/time.h
    struct timeval start, end;

    // Recording the start time(it gets the exact time and store it into start)
    gettimeofday(&start, NULL);

    // Perform the matrix multiplication on the two arrays and store
    // the answer in the array called "result"
    multiplyMatrices(0, array_size, A, B, result);

    // Recording the End time of execution
    gettimeofday(&end, NULL);

    // Calculating the execution time (note: this calculations are defined in the library previously)
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;

    printf("The execution time for the Naive way = %f second\n", elapsed);

    return 0;
}

// the following fill function takes an element which is the number that
// we will repeat it to generate our array
void fillArray(long long elem, int n, int arr[n][n])
{
    long long temp = elem;
    // the integer 'count' represents the number of digits for the element,
    // in out task it's either 7 (if it's the University ID) or 10
    // (if it's ID*Birth Date)
    int count = 0;
    while (temp != 0)
    {
        temp /= 10;
        count++;
    }
    // this array to store the numbers that we are going to fill the array with
    int rep[count];
    for (int i = 0; i < count; i++)
    {
        rep[count - 1 - i] = elem % 10;
        elem /= 10;
    }
    // this pointer is pointing on array elements, when it reaches the last element
    // of the array , it automatically go back and point to the first element
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