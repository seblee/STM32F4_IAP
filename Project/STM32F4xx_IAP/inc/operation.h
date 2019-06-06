
#ifndef _OPERATION_H_
#define _OPERATION_H_

#include <fal.h>

#define FLASH_APP_FLAG_WORD 0xa5a5

typedef struct
{
    uint16_t app_flag;
    uint32_t size;
    char md5[33];
    char version[10];
    char url[256];
} app_struct;
typedef app_struct *app_struct_t;
int Erase_partition(char *name);
int Copy_program(void);
int isthereOTAflag(void);

void _hmac_md5(const char *msg, int msg_len, char *digest, const char *key, int key_len);
void _md5(const unsigned char *input, size_t ilen, unsigned char output[32]);

#endif /* _OPERATION_H_ */
