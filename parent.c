//---------------------------------------------------------------------
// Assignment :   PA2 - IPC
// Date :         10/28/19
// Author :       Nick Albright & Miguel Padilla
// File Name :    parent.c
//----------------------------------------------------------------------
#include <time.h>
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
    lineMsg msgP;
    int lineQueID, msgStatus;
 
    lineQueID = Msgget(LINE_MAILBOX_KEY, IPC_CREAT | 0666);

    pid_t mypid, pid, superID;
    mypid = getpid();
    pid = mypid;

    int semflg, semmode;
    sem_t *startLine, *lineOutput, *linesDone, *printReport, *termLine;
    semflg = O_CREAT;
    semmode = S_IRUSR | S_IWUSR;
    startLine = Sem_open("/startLine", semflg, semmode, 1);
    //termLine = Sem_open("/termLine", semflg, semmode, 0);
    lineOutput = Sem_open("/lineOutput", semflg, semmode, 1);
    linesDone = Sem_open("/linesDone", semflg, semmode, 0);
    printReport = Sem_open("/printReport", semflg, semmode, 0);

    printf("PARENT: Will Manufacture an Order of Size = %d parts\n", order_size);
    printf("Creating %d Factory Lines\n", factory_lines);
    
    int factoryID = 0;
    int fd_line = open("factory.log", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_line == -1)
    {
	    perror("Opening Factory Line's log file failed");
    	exit(-1);
    }

    int randCap;
    int randDur;
    srandom(time(NULL));
    // Make factory lines
    for (int i = 1; i < factory_lines + 1; i++) {
        randCap = random() %41 + 10;
        randDur = random() % 701 + 500;
        if (pid != 0) { pid = Fork(); }
        
        // Child process
        if (pid == 0 )
        {
            printf("PARENT: Factory Line    %d Created with Capacity %d Duration %d\n", i, randCap, randDur);
            dup2(fd_line, 1);
            int semVal = 0;
            Sem_getvalue(startLine, &semVal);
            Sem_post(startLine);
            char str[2];
            if (sprintf(str, "%d", semVal) < 0) {
              fprintf(stderr, "error - not an integer");
            }
            char buf1[50];
            snprintf(buf1, sizeof(buf1), "%d", randCap);
            char buf2[50];
            snprintf(buf2, sizeof(buf2), "%d", randDur);
	        if (execlp("./factory_line",
		        "line.out", str, buf1, buf2, NULL) != -1) { perror("Executing a child failed\n"); }
        } 
    }

    // Supervisor
    superID = Fork();
    if (superID == 0)
    {
	    int fd_sup = open("supervisor.log", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	    if (fd_sup == -1)
	    {
	        perror("Opening Supervisor's log file failed");
    	    exit(-1);
    	}
    	dup2( fd_sup, 1);
        char strS[2];
    	if (snprintf( strS, sizeof(strS), "%d", factory_lines) < 0) {
            perror("Super snprint has failed");
            exit(-1);
    	}
    	if (execlp("./supervisor", "super.out", strS, NULL) < 0) {
    	    perror("PARENT: execlp Supervisor Failed");
	        exit(-1);
    	}
    }


    if (getpid() == mypid) {

        // Wait for supervisor to tell us lines are done
        Sem_wait(linesDone);
        printf("PARENT: Supervisor says all lines have completed\n");
        // Tell supervisor lines are done

        Sem_post(printReport);
        for (int i = 0; i < factory_lines; i++ )
        //Sem_post(termLine);
	    // Wait for supervisor to finish
        if (waitpid(superID, NULL, 0) == -1 ) { perror("supervisor wait failed"); exit(-1); }
        printf("PARENT: Shutting Down Factory Lines\n");

        pid_t wpid;
        int status = 0;
        // This is not a final solution > just making sure all processes finish
	    for (int i = 0; i < factory_lines; i++) {
	        if (wait(NULL) == -1) { perror("line wait failed"); exit(-1); }
	    }

        // Remove/destroy IPC things
        shmdt(p);
        Sem_close(startLine);
        Sem_unlink("/startLine");
        Sem_close(lineOutput);
        Sem_unlink("/lineOutput");
        Sem_close(linesDone);
        Sem_unlink("/linesDone");
        Sem_close(printReport);
        Sem_unlink("/printReport");

        if (msgctl(lineQueID, IPC_RMID, NULL) == -1) {
	    	fprintf(stderr, "Line message queue could not be deleted.\n");
	    	exit(EXIT_FAILURE);
        }

    }

    return EXIT_SUCCESS;

}
