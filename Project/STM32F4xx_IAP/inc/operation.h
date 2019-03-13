
#ifndef _OPERATION_H_
#define _OPERATION_H_

#include <fal.h>

#define FLASH_APP_FLAG_WORD 0xa5a5

typedef struct
{
    uint16_t app_flag;
    uint32_t app_len;
    char md5[32];
} app_struct;

int Erase_partition(char *name);
int Copy_program(void);
typedef app_struct *app_struct_t;

#endif /* _OPERATION_H_ */
