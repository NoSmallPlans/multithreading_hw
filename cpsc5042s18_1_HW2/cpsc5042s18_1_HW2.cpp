#include <pthread.h>
#include <array>
#include <iostream>

// AUTHOR: Kevin Cushing
// FILENAME: cpsc5042s18_1_HW2.cpp
// DATE: 5/21/2018
// REVISION HISTORY: 0

/** OVERVIEW
 *
 * Requires command line arguments
 *
 * FUNCTIONALITY:
 * A multithreaded program that calculates various statistical values for
 * a list of numbers. This program will be passed a series of numbers on
 * the command line and will then create three separate worker threads.
 * One thread will determine the average of the numbers, the second will
 *
 * PLEDGE:
 * I have not received unauthorized aid on this assignment.
 * I understand the answers that I have submitted. The answers
 * submitted have not been directly copied from another source,
 * but instead are written in my own words.
 *
 **/

/*
 * Global variables to be accessed by threads
 */
int min;
int max;
int avg;
int inputSize;

/*
 * Find the mean value in array of ints
 */
void *get_avg(void *arg)
{
    int* inArray = (int*)arg;
    int sum = 0;
    int denominator = inputSize;
    avg = 0;
    for(int i = 1; i < inputSize; i++){
        sum += inArray[i];
    }
    avg = sum/denominator;
    pthread_exit(NULL);
}

/*
 * Find the minimum value in array of ints
 */
void *get_min(void *arg)
{
    int* inArray = (int*)arg;
    min = inArray[1];
    for(int i = 1; i < inputSize; i++){
        if(inArray[i] < min) min = inArray[i];
    }
    pthread_exit(NULL);
}

/*
 * Find the maximum value in array of ints
 */
void *get_max(void *arg)
{
    int* inArray = (int*)arg;
    max = inArray[0];
    for(int i = 1; i < inputSize; i++){
        if(inArray[i] > max) max = inArray[i];
    }
    pthread_exit(NULL);
}

/*
 * Handle errors when creating threads fails
 */
void tryCreate(int ret){
    if(ret != 0) {
        printf("Error: pthread_create() failed\n");
        exit(EXIT_FAILURE);
    }
}


int main(int argc,char* argv[])
{
    //int numbers[N] = {90,81,78,95,79,72,85};
    if (argc > 1) {
        inputSize = argc;
        int numbers[inputSize];
        for (int i = 0; i < inputSize; i++) {
            numbers[i] = atoi(argv[i]);
        }
        int threadCount = 3;
        pthread_t threads[threadCount];

        tryCreate(pthread_create(&threads[0], NULL, &get_avg, (void *) numbers));
        tryCreate(pthread_create(&threads[1], NULL, &get_min, (void *) numbers));
        tryCreate(pthread_create(&threads[2], NULL, &get_max, (void *) numbers));

        for (int i = 0; i < threadCount; i++) {
            // block until thread 'index' completes
            int ret = pthread_join(threads[i], NULL);
            if (ret != 0) {
                std::cout << "Error joining threads" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        std::cout << "Average = " << avg << std::endl;
        std::cout << "Max = " << max << std::endl;
        std::cout << "Min = " << min << std::endl;
        printf("In main: All threads completed successfully\n");
        exit(EXIT_SUCCESS);
    } else {
        printf("This program requires integer command line arguments, please try again.\n");
        exit(EXIT_SUCCESS);
    }
}