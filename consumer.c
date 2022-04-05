#include "master.h"

int main()
{
    int fd;
    fd = shm_open(SHM_REQUEST, O_RDWR,
        0666);
    if (fd == -1)
        errExit("shm_open error");

    struct table* tbl = mmap(NULL, sizeof(*tbl),
        PROT_READ | PROT_WRITE,
        MAP_SHARED, fd, 0);
    
    if (sem_post(&tbl->empty) == -1)
        errExit("sem_post");

    if (sem_wait(&tbl->full) == -1)
        errExit("sem_wait");

    do
    {
        int consumed;
        sem_wait(&tbl->full); // is initiated to 0 in producer.c
			      // wait operation on full semaphore
			      // is saying wait until full > 0
			      // which doesn't occur until the end of the producer loop
			      // when sem_post(&tbl->full) increments Semaphore(full)

        pthread_mutex_lock(&tbl->mutexControl);
				// aquires the lock from the binary semaphore
				// ensuring that producer cannot access critical section
        printf("\n***Consumer entering critical section***");

        while (tbl->counter > 0) {      
            tbl->counter--;
            consumed = tbl->buffer[tbl->counter];
            printf("\nConsumed %d", consumed);
            tbl->buffer[tbl->counter] = 0;
        }

        printf("\n***Consumer exiting critical section***\n");

        pthread_mutex_unlock(&tbl->mutexControl);
				// releases the lock from the binary semaphore
				// allowing the producer to access its critical section
        sem_post(&tbl->empty); 
        
    } while (1);
}