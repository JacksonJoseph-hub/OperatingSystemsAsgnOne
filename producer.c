#include "master.h"


int main()
{
    srand(time(0)); //Randomize producer item	
    /* Create shared memory object and set its size */
    /*
    * Format for shared memory taken mostly from Linux manual example provided in the powerpoint, on:
    https://man7.org/linux/man-pages/man3/shm_open.3.html
    */

    int fd = shm_open(SHM_REQUEST, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
        errExit("shm_open error");

    if (ftruncate(fd, sizeof(struct table)) == -1)
        errExit("ftruncate");


    /* Create shared memory object between producer.c & consumer.c */

    struct table* tbl = mmap(NULL, sizeof(*tbl),
        PROT_READ | PROT_WRITE,
        MAP_SHARED, fd, 0);

    if (tbl == MAP_FAILED)
        errExit("mmap error");
	
	//initialize Semaphore(empty) to the size (tableSize) of the array, in this case two elements
    if (sem_init(&tbl->empty, 1, tableSize) == -1)
        errExit("sem_init-empty error");

	//initialize Semaphore(full) to zero, ensuring the consumer doesnt access its critical section
	//while the table has no data
    if (sem_init(&tbl->full, 1, 0) == -1)
        errExit("sem_init-full error");

	//initialize binary semaphore (mutexControl) to NULL, which sets the mutex attributes to default and unlocks it
    pthread_mutex_init(&tbl->mutexControl, NULL);

    if (sem_wait(&tbl->empty) == -1)
        errExit("sem_wait");

    tbl->counter = 0; //Initialize the counter used to index the shared array to zero
    do
    {
        sem_wait(&tbl->empty);
        pthread_mutex_lock(&tbl->mutexControl);
		// aquires the lock from the binary semaphore
		// ensuring that producer cannot access critical section
        printf("\n***Producer entering critical section***");

        while (tbl->counter < tableSize) {

            int produced = rand() % maxElementValue + 1;
            printf("\nProduced %d", produced);
            tbl->buffer[tbl->counter] = produced;
            tbl->counter++;         
        }
        printf("\n***Producer exiting critical section***\n");
        pthread_mutex_unlock(&tbl->mutexControl);
		// releases the lock from the binary semaphore
		// allowing the producer to access its critical section
        sem_post(&tbl->full);
		// increment Semaphore(full), allowing the consumer to enter critical section
        
    } while(1);
    shm_unlink(SHM_REQUEST);
}
