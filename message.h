//---------------------------------------------------------------------
// Assignment :   PA2 - IPC
// Date :         10/28/19
// Author :       Nick Albright & Miguel Padilla
// File Name :    message.h
//----------------------------------------------------------------------

#include <sys/types.h>

#define LINE_MAILBOX_KEY 0x20
 
typedef struct {
    int line_id ;   // factory line id
    int sender_pid;     /* Sender places their process id here */  
    int msgTyp; // 0 = production msg, 1 = completion msg
    int capacity; // p
    int num_parts; // p
    int duration; // p
    int parts_made;  // c
    int iterations; // c
} lineMsg ;

#define LINE_MSG_SIZE ( sizeof(lineMsg) )
#define FINAL_MESSAGE_LEN 82

