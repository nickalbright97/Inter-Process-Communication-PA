//---------------------------------------------------------------------
// Assignment :   PA2 - IPC
// Date :         10/28/19
// Author :       Nick Albright & Miguel Padilla
// File Name :    parent.c
//----------------------------------------------------------------------

#include "wrappers.h"

int main(int argc, char **argv)
{
    int factory_lines, order_size;
    if (argc != 3) { perror("Must be two arguments: number of factory lines and order size"); exit(EXIT_FAILURE); }
    
    // Parse arguments from command line
    if (sscanf (argv[1], "%i", &factory_lines) != 1) {
        fprintf(stderr, "error - not an integer");
    }

    if (sscanf (argv[2], "%i", &order_size) != 1) {
        fprintf(stderr, "error - not an integer");
    }
    
    // Check for valid integers
    if (factory_lines < 0 || order_size < 0) {
        perror("Invalid integer entry on command line");
        exit(-1);
    }

    // shared memory
    int shmid, shmflg; 
    key_t shmkey;
    shmData *p;
    shmflg = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;

    shmkey = Ftok("shmIPC.h", PROJ_ID);
    shmid = Shmget(shmkey, SHMEM_SIZE, shmflg);
    p = Shmat(shmid, NULL, 0);
    
    p->order_size = order_size;
    p->parts_made = 0;
    p->parts_remaining = order_size;

    // message queue
    productionMsg msgP;
    completionMsg msgC;
    int prodQueID;
    int compQueID;
    int msgStatus;
    
    prodQueID = msgget(PROD_MAILBOX_KEY, IPC_CREAT);
    compQueID = msgget(COMP_MAILBOX_KEY, IPC_CREAT);

    pid_t mypid, pid;
    mypid = getpid();
    
    pid = fork();
	if ( pid < 0 ) 
	{ /* error occurred */
		fprintf(stderr, "FORK FAILED");
		exit(-1);
	} else if (pid == 0)
    {
        printf("\n\t\tthis is the child process with id = %d\n" , getpid() );
        if ( execlp( "/cs/home/stu/albrigne/Documents/CS361/PAs/PA2/Inter-Process-Communication-PA/factory_line" ,
                 "factory_line", "1", "10", "10", NULL ) < 0 )
            perror( "Executing a child failed\n" );
        exit(0); // not sure if this is needed
    } else {
        wait(NULL);
        printf ("\nMY CHILD HAS COMPLETED\n");
    }
    
    

    // Remove/destroy IPC things
    shmdt(p);
    
    if (msgctl(prodQueID, IPC_RMID, NULL) == -1) {
		fprintf(stderr, "Production message queue could not be deleted.\n");
		exit(EXIT_FAILURE);
	}
    if (msgctl(compQueID, IPC_RMID, NULL) == -1) {
		fprintf(stderr, "Completion message queue could not be deleted.\n");
		exit(EXIT_FAILURE);
	}
    
    

    return EXIT_SUCCESS;
    
    

    






}
