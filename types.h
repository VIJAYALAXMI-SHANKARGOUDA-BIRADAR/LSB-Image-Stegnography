#ifndef TYPES_H
#define TYPES_H

/*#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)  // Only define once
#define MAX_FILE_SUFFIX 5
#define MAX_MAGIC_STRING_SIZE 2*/


/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure,

    //d_success,
    //d_failure
} Status;

typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif
