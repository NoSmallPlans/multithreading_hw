#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <iostream>

// AUTHOR: Kevin Cushing
// FILENAME: cpsc5042s18_3_HW2.cpp
// DATE: 5/21/2018
// REVISION HISTORY: 0

/** OVERVIEW
 *
 * create 4 threads in your program and assign ID 1,2,3,4 to them respectively.
 * Each of the threads will try to access the variable “CurrentID”. Whenever a
 * thread acquires the variable, it checks whether the CurrentId is equal to its
 * own Id or not. If it is not equal, it will output “Not My Turn!”, then print
 * its threadId, and then release the variable. If it is equal, the thread will
 * print “My turn!”, then print its threadId, increase the CurrentId by 1, and
 * then release the variable. However, after increasing CurrentID by 1, the
 * thread will check if the value is 5 or not. If it is 5, it will reset it to
 * 1 before releasing the variable. The program should execute until each
 * thread prints “MyTurn!” 10 times. Once a thread prints for 10 times, it
 * terminates.
 *
 * PLEDGE:
 * I have not received unauthorized aid on this assignment.
 * I understand the answers that I have submitted. The answers
 * submitted have not been directly copied from another source,
 * but instead are written in my own words.
 *
 **/


/*
 * Program Global Variables
 */
const int THREAD_COUNT = 4;
pthread_t threads[THREAD_COUNT];
pthread_mutex_t lock;
int currentID = 1; // start with an ID of 1

void tryCreate(int ret){
    if(ret != 0) {
        printf("Error: pthread_create() failed\n");
        exit(EXIT_FAILURE);
    }
}


/*
 * Incrementer function is run by each thread
 * Each thread attempts to access the mutex lock
 * If locked it checks whether it's current ID is ready
 * If currently ready for this threads ID it will increment ID
 * IDs reset at 5
 * Program ends after each thread iterates 10x
 */
void* incrementer(void *arg){
    int id = (intptr_t)arg;
    int printCounter = 0;
    const int MAX_PRINT = printCounter + 10;
    id++; // start id's at 1 instead of 0


    while(printCounter < MAX_PRINT) {
        pthread_mutex_lock(&lock);
        if (id != currentID) {
            printf("\nNot my turn!\n");
            printf("My thread ID is %d \n", id);
        } else {
            printf("\nMy turn!\n");
            printf("My thread ID is %d \n", id);
            currentID++;
            if (currentID > THREAD_COUNT) currentID = 1;
            printCounter++;
        }
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}


int main(void){
    //Create mutex lock
    if (pthread_mutex_init(&lock, NULL) != 0){
        std::cout << "mutex init has failed" << std:: endl;
        return 1;
    }

    //Create Threads
    for(int i = 0; i < THREAD_COUNT; i++){
        tryCreate(pthread_create(&threads[i], NULL, &incrementer, (void *)(intptr_t)i));
    }

    //Join Threads
    for(int i = 0; i < THREAD_COUNT; i++){
        pthread_join(threads[i], NULL);
    }

    //End Mutex Lock
    pthread_mutex_destroy(&lock);
    return 0;
}