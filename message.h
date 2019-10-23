//---------------------------------------------------------------------
// Assignment :   PA2 - IPC
// Date :         10/28/19
// Author :       Nick Albright & Miguel Padilla
// File Name :    message.h
//----------------------------------------------------------------------

#include <sys/types.h>

#define PROD_MAILBOX_KEY 0x20
#define COMP_MAILBOX_KEY 0x40

typedef struct {
    int line_id ;   // factory line id
    int sender_pid;     /* Sender places their process id here */
    int capacity; 
    int num_parts; // # of parts just completed
    int duration; // time it took to make those parts
} productionMsg ;

#define PROD_MSG_SIZE ( sizeof(productionMsg) )
 
typedef struct {
    int line_id ;   // factory line id
    int sender_pid;     /* Sender places their process id here */
    int parts_made; 
    int iterations;
} completionMsg ;

#define COMP_MSG_SIZE ( sizeof(completionMsg) )


