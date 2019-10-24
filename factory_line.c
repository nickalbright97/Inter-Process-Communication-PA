#include "wrappers.h"

int main(int argc, char **argv)
{
    int iterations, partsMadeByMe, producing = 0;

    int factory_ID;
    int capacity;
    int duration;
    int remain; 
    
    FILE *f = fopen("factory.log", "r+");
    freopen("factory.log", "a+", stdout);
    
    if (argc != 4) { perror("Must have 3 arguments, factory ID, capacity, and duration"); exit(EXIT_FAILURE); }

    // add error checking here > like in parent.c
    if (sscanf (argv[1], "%i", &factory_ID) != 1) {
        fprintf(stderr, "error - not an integer");
    }

    if (sscanf (argv[2], "%i", &capacity) != 1) {
        fprintf(stderr, "error - not an integer");
    }

    if (sscanf (argv[3], "%i", &duration) != 1) {
        fprintf(stderr, "error - not an integer");
    }

    if (factory_ID < 0 || capacity < 0 || duration < 0) {
        perror("Invalid integer entry on command line");
        exit(-1);
    }


    int shmid, shmflg;
    key_t shmkey;
    shmData *p;
    shmflg = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;

    shmkey = Ftok("shmIPC.h", PROJ_ID);
    shmid = Shmget(shmkey, SHMEM_SIZE, shmflg);    
    
    p = Shmat(shmid, NULL, 0);

    
    while(p->parts_remaining > 0) { 
        producing = 0;
    //mutex?
        if (capacity > p->parts_remaining) 
        {
            producing = p->parts_remaining;
            p->parts_remaining = 0;
        }
        else
        {
            producing = capacity;
            p->parts_remaining -= producing;
        }
    //release mutex

        printf( "Factory Line   %d: Going to make    %d parts in  %d milliSecs\n",
		 factory_ID, producing, duration);
	    iterations++;
	    partsMadeByMe += producing;
        usleep(duration);
        
    }



    printf("Factory Line   %d: Terminating after making total of    %d parts in  %d iterations\n", 
        factory_ID,  partsMadeByMe, iterations);

    exit(0);



}

