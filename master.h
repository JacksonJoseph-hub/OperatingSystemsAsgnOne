// Jackson Joseph
// 3 - 31 - 2022
// CS 35003 Operating Systems
// Assignment One - Producer-Consumer buffer with shared memory


#ifndef  MASTER_H
#define MASTER_H

#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define BUFF_SIZE 1024
#define tableSize 2
#define maxElementValue 300
#define SHM_REQUEST "/myTable"
#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                                   } while (0)
//Shared memory object
struct table {
    sem_t empty;
    sem_t full;
    pthread_mutex_t mutexControl; //acquire and release synch lock
    int buffer[tableSize];
    int counter;
};


#endif // ! MASTER_H
