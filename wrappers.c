//---------------------------------------------------------------------
// Assignment :   PA2 - IPC
// Date :         10/28/19
// Author :       Nick Albright & Miguel Padilla
// File Name :    wrappers.c
//----------------------------------------------------------------------
#include "wrappers.h"

pid_t Fork(void) {
    pid_t pid;
    pid = fork();
    if ( pid < 0 ) 
	{ /* error occurred */
		fprintf(stderr, "FORK FAILED");
		exit(-1);
	}
}

void unix_error(char *msg) {
    perror(msg);
    exit(-1);
}

int Shmget(key_t key, size_t size, int shmflg) {
    int shmid = shmget (key, size, shmflg);
    if (shmid == -1) {
        printf("\nFailed to get shared memory id = %d\n", shmid);
        perror("Reason: ");
        exit(-1);
    }
}

shmData * Shmat(int shmid, const void *shmaddr, int shmflg) {
    shmData *p;
    p = (shmData *) shmat(shmid, shmaddr, shmflg);
       if (p == (shmData*) -1) {
       printf ("\nFailed to attach shared memory id=%d\n", shmid);
       perror("Reason: ");
       exit(-1);
   }
}

sem_t * Sem_open( const char *name, int oflag, mode_t mode, unsigned int value ) {
   sem_t * sem = sem_open(name, oflag, mode, value);
   if (sem == SEM_FAILED) { perror("Sem_open failed"); exit(-1); }
}

sem_t * Sem_open2( const char *name, int oflag) {
   sem_t * sem = sem_open(name, oflag);
   if (sem == SEM_FAILED) { perror("Sem_open failed"); exit(-1); }
}

int Sem_close( sem_t *sem ) {
    int r = sem_close(sem);
    if (r == -1) {
        perror("Sem_close failed");
        exit(-1);
    }
}

int Sem_unlink( const char *name ) {
    int r = sem_unlink(name);
    if (r == -1) {
        perror("Sem_unlink failed");
        exit(-1);
    }
}

int Sem_wait( sem_t *sem ) {
    int r = sem_wait(sem);
    if (r == -1) {
        perror("Sem_wait failed");
        exit(-1);
    }
}

int Sem_post( sem_t *sem ) {
    int r = sem_post(sem);
    if (r == -1) {
        perror("Sem_post failed");
        exit(-1);
    }
}

int Sem_init( sem_t *sem, int pshared, unsigned int value ) {
    int r = sem_init(sem, pshared, value);
    if (r == -1) {
        perror("Sem_init failed");
        exit(-1);
    }
}

int Sem_destroy( sem_t *sem ) {
    int r = sem_destroy(sem);
    if (r == -1) {
        perror("Sem_destroy failed");
        exit(-1);
    }
}

int Msgget(key_t key, int msgflg) {
    int r = msgget(key, msgflg);
    if (r == -1) {
        perror("Msgget failed");
        exit(-1);
    }
}


key_t Ftok(char *pathname, char proj_id) {
    int key;
    key = ftok(pathname, proj_id);
    if (key == -1) {
        perror("Frok failed");
        exit(-1);
    }
}







