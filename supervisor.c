//---------------------------------------------------------------------
// Assignment :   PA2 - IPC
// Date :         10/28/19
// Author :       Nick Albright & Miguel Padilla
// File Name :    supervisor.c
//----------------------------------------------------------------------

#include "wrappers.h"

int main (int argc, char **argv) 
{
    int lineQueID, linesActive; 
    
    // shared memory
    int shmid, shmflg; 
    key_t shmkey;
    shmData *p;
    shmflg = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | IPC_CREAT;

    shmkey = Ftok("shmIPC.h", PROJ_ID);
    shmid = Shmget(shmkey, SHMEM_SIZE, shmflg);
    p = Shmat(shmid, NULL, 0);

    int semflg, semMode;
    sem_t *linesDone, *printReport, *startLine; 
    
    semflg = O_CREAT;
    semMode = S_IRUSR | S_IWUSR;

    linesDone = Sem_open("/linesDone", semflg, semMode, 0);
    printReport = Sem_open("/printReport", semflg, semMode, 0);
    startLine = Sem_open2("/startLine", 0); // Arg of 0 means sem value is not overwritten

    int msgStatus;
    lineMsg lineMsg;
    lineQueID = Msgget(LINE_MAILBOX_KEY, IPC_CREAT | 0666);
    
    if (sscanf (argv[1], "%i", &linesActive) == -1) {
        fprintf(stderr, "error - not an integer");
    }

    char report[FINAL_MESSAGE_LEN * linesActive];
    char lineStr[FINAL_MESSAGE_LEN];

    
    int reportPos = 0;
    int linePos = 0;
    while(linesActive > 0) {
        msgStatus = msgrcv(lineQueID, &lineMsg, LINE_MSG_SIZE, 0, 0);
        if (msgStatus == -1) {
             perror("Line Message reception failed in supervisor process");
             exit(-1);
        }
        if (lineMsg.msgTyp == 0) {
             printf("Factory Line   %d Produced   %d parts in   %d milliSecs\n", lineMsg.line_id, 
                    lineMsg.num_parts, lineMsg.duration);
        } else if (lineMsg.msgTyp == 1) {
            printf("Factory Line   %d Completed its task\n", lineMsg.line_id);
            linesActive--;
            snprintf(lineStr, FINAL_MESSAGE_LEN, "Line    %d made total of %d parts in     %d iterations", 
                        lineMsg.line_id, lineMsg.num_parts, lineMsg.iterations);
            lineStr[ strlen(lineStr)-1 ] = '\n' ; // get rid of the new line character
            for (int i = reportPos; i < reportPos + FINAL_MESSAGE_LEN; i++) {
                report[i] = lineStr[linePos];
                linePos++;
            }
            linePos = 0;
            reportPos += FINAL_MESSAGE_LEN;
      //      printf("lineStr: %s \n\n", lineStr);
      //      printf("report: %s \n", report);
        }
    }
    // Tell parent lines are done
    Sem_post(linesDone);
    // Wait for parent to tell us to print report
    Sem_wait(printReport);
    printf("\n****** SUPER: Final Report ******\n");
    

    shmdt(p);
    Sem_close(startLine);
    Sem_close(linesDone);
    Sem_close(printReport);
    // Close other semaphores? - close semaphore in every process, unlink once

}
