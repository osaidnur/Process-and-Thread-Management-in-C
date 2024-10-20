### Process-and-Thread-Management-in-C
This repository contains a C program that demonstrates matrix multiplication using different approaches, highlighting the impact of optimization techniques such as multiprocessing and multithreading. We will compare the performance of:

1. **Naive Matrix Multiplication**: A straightforward, single-threaded implementation.
2. **Multiprocessing**: Matrix multiplication divided across multiple processes.
3. **Multithreading (Joinable Threads)**: Parallel matrix multiplication using joinable threads.
4. **Multithreading (Detached Threads)**: Parallel matrix multiplication using detached threads.

## Features

- **Naive Implementation**: The traditional approach to matrix multiplication, iterating over the rows and columns in a nested loop structure.
- **Multiprocessing**: Utilizes multiple processes to handle different portions of the matrix multiplication task, improving performance by distributing the workload.
- **Multithreading**:
  - **Joinable Threads**: Threads are created to perform matrix multiplication concurrently, and the main thread waits for them to finish.
  - **Detached Threads**: Threads run independently without requiring the main thread to wait, increasing potential parallelism but requiring careful management of resources.

## Performance Comparison

For each method, the program measures the execution time and throughput, demonstrating the advantages and trade-offs of using multiprocessing and multithreading.

## Results

The program will output the time taken for matrix multiplication using each method, allowing you to observe the impact of process and thread management on performance.

