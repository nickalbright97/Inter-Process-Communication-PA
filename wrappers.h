/************************************************
 * Wrappers for system call functions
 ************************************************/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/stat.h>    
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/msg.h>
#include "shmIPC.h"
#include "message.h"

pid_t   Fork(void);
void    unix_error(char *msg) ;
int     Shmget(key_t key, size_t size, int shmflg);
shmData *Shmat(int shmid, const void *shmaddr, int shmflg);
int     Shmdt( const void *shmaddr ) ;
sem_t  *Sem_open( const char *name, int oflag, mode_t mode, unsigned int value );
sem_t  *Sem_open2( const char *name, int oflag ) ;
int     Sem_close( sem_t *sem );
int     Sem_unlink( const char *name );
int     Sem_getvalue(sem_t *sem, int *sval);
int     Sem_wait( sem_t *sem ) ;
int     Sem_post( sem_t *sem ) ;
int     Sem_init( sem_t *sem, int pshared, unsigned int value ) ;
int     Sem_destroy( sem_t *sem ) ;
int     Msgget( key_t key, int msgflg );
key_t   Ftok( char *pathname, char proj_id);


