#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <getopt.h>



int main(int argc, char **argv)
{
    int iterations, partsMadeByMe = 0;

    int factory_ID;
    int capacity;
    int duration;
    int remain; 

    FILE *f = fopen("factory.log", "r+");
    use(f);

    factory_ID = atoi(argv[1]);
    capacity = atoi(argv[2]);
    duration = atoi(argv[3]);
    
    int shmid, shmflg; 
    key_t shmkey;
    shmData *p;
    shmflg = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;

    shmkey = Ftok("shmIPC.h", PROJ_ID);
    shmid = Shmget(shmkey, SHMEM_SIZE, shmflg);    
    
    p = Shmat(shmid, NULL, 0);

    while(p->parts_remaining > 0) { 
        int producing;
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

        fprintf(f, "Factory Line   %d: Going to make    %d parts in  %d milliSecs", myID, data, duration);
        usleep(duration);
        
        iterations ++;
        partsMadeByMe += producing;
    }


    fprintf(f, "Factory Line   %d: Terminating after making total of    %d parts in  %d iterations",         myID,  partsMadeByMe, iterations);



}

