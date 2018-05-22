#include <iostream>

// AUTHOR: Kevin Cushing
// FILENAME: cpsc5042s18_2_HW2.cpp
// DATE: 5/21/2018
// REVISION HISTORY: 0

/** OVERVIEW
 *
 * Requires command line arguments
 *
 * FUNCTIONALITY:
 * This program accepts command line arguments and sorts them
 * Sorting is done via two threads that split the problem in half
 *
 * PLEDGE:
 * I have not received unauthorized aid on this assignment.
 * I understand the answers that I have submitted. The answers
 * submitted have not been directly copied from another source,
 * but instead are written in my own words.
 *
 **/


/*
 * Struct for passing data to thread runner function
 */
struct SortData {
    int * dataArray;
    int * outputArray;
    int startIndex;
    int inputSize;
};

/*
 * Handle errors from creating threads
 */
void tryCreate(int ret){
    if(ret != 0) {
        printf("Error: pthread_create() failed\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * Sort the individual segments of the array.
 * Simple sort. Ala selection sort
 */
void *sort(void *arg)
{
    struct SortData *input = (struct SortData *)arg;
    int endIndex = input->startIndex+1 + (input->inputSize/2);
    int startIndex = input->startIndex+1;
    int smallest;

    for(int i=startIndex; i < endIndex; i++){
        smallest = input->dataArray[i];
        for(int k = i+1; k < endIndex; k++) {
            if (smallest > input->dataArray[k]) {
                input->dataArray[i] = input->dataArray[k];
                input->dataArray[k] = smallest;
                smallest = input->dataArray[i];
            }
        }
    }

    pthread_exit(NULL);
}


/*
 * Merge Sort adapted from referencing https://www.geeksforgeeks.org/merge-sort/
 * Brings the two sorted segments together
 */
void *merge(void *arg)
{
    struct SortData *input = (struct SortData *)arg;
    int endIndex = input->startIndex + input->inputSize;
    int startIndex = input->startIndex+1;
    int midPoint = input->startIndex + (input->inputSize/2);
    int * dataArray = input->dataArray;
    int * outputArray = input->outputArray;

    int i = 0;
    int j = midPoint+1;
    int k = 0;
    while(i < midPoint && j < endIndex) {
        if(dataArray[i] <= dataArray[j]){
            outputArray[k] = dataArray[i];
            i++;
        } else {
            outputArray[k] = dataArray[j];
            j++;
        }
        k++;
    }

    while(i < midPoint+1) {
        outputArray[k] = dataArray[i];
        i++;
        k++;
    }

    while(j < endIndex) {
        outputArray[k] = dataArray[j];
        j++;
        k++;
    }

    pthread_exit(NULL);
}


/*
 * Main runner function
 */
int main(int argc,char* argv[])
{
    //check for program arguments
    //If not, notify user of requirements
    if(argc > 1) {
        int inputSize = argc;
        int threadCount = 2;
        int halfSize = inputSize / 2;
        int numbers[inputSize];
        int result[inputSize];
        for (int i = 0; i < inputSize; i++) {
            numbers[i] = atoi(argv[i]);
        }

        //Create structures for each thread house data
        struct SortData first;
        first.startIndex = 0;
        first.dataArray = numbers;
        first.inputSize = inputSize;
        struct SortData second;
        second.startIndex = halfSize;
        second.dataArray = numbers;
        second.inputSize = inputSize;
        struct SortData third;
        third.startIndex = 0;
        third.dataArray = numbers;
        third.outputArray = result;
        third.inputSize = inputSize;

        pthread_t threads[threadCount];

        //create two sorting threads
        tryCreate(pthread_create(&threads[0], NULL, &sort, (void *) &first));
        tryCreate(pthread_create(&threads[1], NULL, &sort, (void *) &second));

        for (int i = 0; i < threadCount; i++) {
            // block until thread 'index' completes
            int ret = pthread_join(threads[i], NULL);
            if (ret != 0) {
                std::cout << "Error joining threads" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        //create a merging thread
        tryCreate(pthread_create(&threads[2], NULL, &merge, (void *) &third));
        int res = pthread_join(threads[2], NULL);
        if (res != 0) {
            std::cout << "Error joining threads" << std::endl;
            exit(EXIT_FAILURE);
        }

        //print results of sorting and merging
        for (int z = 1; z < inputSize; z++) {
            printf("%d ", result[z]);
        }
        printf("\n");
    } else {
        std::cout << "This program requires command line arguments on launch. Please try again" << std::endl;
    }
    return 0;
}