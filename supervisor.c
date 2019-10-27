//---------------------------------------------------------------------
// Assignment :   PA2 - IPC
// Date :         10/28/19
// Author :       Nick Albright & Miguel Padilla
// File Name :    supervisor.c
//----------------------------------------------------------------------

#include "wrappers.h"

int main () 
{
    int queID; 
    int LinesActive;


    // shared memory
    int shmid, shmflg; 
    key_t shmkey;
    shmData *p;
    shmflg = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;

    shmkey = Ftok("shmIPC.h", PROJ_ID);
    shmid = Shmget(shmkey, SHMEM_SIZE, shmflg);
    p = Shmat(shmid, NULL, 0);

    int semflg, semMode;
    sem_t *parentSem, *factorySem; 
    
    semflg = O_CREAT;
    semMode = S_IRUSR | S_IWUSR;

    freopen("supervisor.log", "a+", stdout);

    parentSem = sem_open("/parent_semaphore", semflg, semMode, 1);

    factorySem = sem_open("/factory_semaphore", semflg, semMode, 1);


    queID = msgget(PROD_MAILBOX_KEY, IPC_CREAT);
    if ( queID == -1 ) 
    {
        printf("Supervisor Failed to create mailbox. Error code=%d\n" 
                , errno ) ;
        perror("Reason");
        exit(-2) ;      
    }

    if (sscanf (argv[1], "%i", &LinesActive) != 1) {
        fprintf(stderr, "error - not an integer");
    }

    while(LinesActive > 0) {
        

        printf("Factory Line   %d Produced   %d parts in   %d milliSecs");
        printf("Factory Line   %d Completed its task");

    }

    printf("Line   %d made total of %d parts in     %d iterations");
}
