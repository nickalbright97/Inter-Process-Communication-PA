#include "wrappers.h"

int main(int argc, char **argv)
{
    int iterations, partsMadeByMe, producing = 0;

    int factory_ID;
    int capacity;
    int duration;
    int remain; 
    
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

    lineMsg lineMsg;
    pid_t mypid = getpid();
    if (mypid < 0) { perror("line pid error"); exit(-1); }
    // Initialize messages & queue
    
    lineMsg.line_id = factory_ID;
    lineMsg.sender_pid = mypid;
    lineMsg.capacity = capacity;
    lineMsg.duration = duration;
    
    int lineQueID, msgStatus;
    lineQueID = Msgget(LINE_MAILBOX_KEY, IPC_CREAT | 0666);

    // Shared memory
    int shmid, shmflg;
    key_t shmkey;
    shmData *p;
    shmflg = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;

    shmkey = Ftok("shmIPC.h", PROJ_ID);
    shmid = Shmget(shmkey, SHMEM_SIZE, shmflg);
    
    p = Shmat(shmid, NULL, 0);

    sem_t *lineOutput;
    lineOutput = Sem_open2("/lineOutput", 0);
    
    while(p->parts_remaining > 0) { 
        Sem_wait(lineOutput);
        producing = 0;
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

        lineMsg.msgTyp = 0;
        lineMsg.num_parts = producing;
        msgStatus = msgsnd(lineQueID, &lineMsg, LINE_MSG_SIZE, 0);
        if (msgStatus == -1) {
            perror("Production msg send failed in factory line process");
            exit(-1);
        }
   
        printf( "Factory Line   %d: Going to make    %d parts in  %d milliSecs\n",
		 factory_ID, producing, duration);
        
        Sem_post(lineOutput);
	    iterations++;
	    partsMadeByMe += producing;
        usleep(duration);
        
    }
    lineMsg.msgTyp = 1;
    lineMsg.parts_made = partsMadeByMe;
    lineMsg.iterations = iterations;
      
    msgStatus = msgsnd(lineQueID, &lineMsg, LINE_MSG_SIZE, 0);
    if (msgStatus == -1) {
         perror("Completion msg send failed in factory line process");
         exit(-1);
    }
    printf("Factory Line   %d: Terminating after making total of    %d parts in  %d iterations\n", 
         factory_ID,  partsMadeByMe, iterations);

    Sem_close(lineOutput);
 
    exit(0);



}

