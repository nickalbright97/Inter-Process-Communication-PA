/*-------------------------------------------------------------------------------
IPC using Shared Memory for PA2
Written By: 
     1- Nick Albright
-------------------------------------------------------------------------------*/

#ifndef SHMEM_IPC
#define SHMEM_IPC


typedef struct {
    int order_size;
    int parts_made;
    int parts_remaining;
} shmData ;

#define SHMEM_SIZE sizeof(shmData)
#define PROJ_ID 13

#endif
