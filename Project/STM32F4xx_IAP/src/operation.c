#include "operation.h"
#include "string.h"
#include "crypto.h"
#include "stm32f4xx.h"

int Erase_partition(char *name)
{
    const struct fal_partition *partition = NULL;
    partition = fal_partition_find(name);
    if (partition == NULL)
    {
        log_e("connot find the part:%s", name);
        return -1;
    }
    return fal_partition_erase_all(partition);
}

int Copy_program(void)
{
    int rc;
    app_struct app_info;
    app_struct_t app_info_p;
    const struct fal_partition *app_partition = NULL;
    const struct fal_partition *download_partition = NULL;
    app_partition = fal_partition_find("app");
    if (app_partition == NULL)
    {
        log_e("connot find the part app_partition");
        return -1;
    }
    download_partition = fal_partition_find("download");
    if (download_partition == NULL)
    {
        log_e("connot find the part download_partition");
        return -1;
    }
    app_info.app_flag = FLASH_APP_FLAG_WORD;
    app_info.size = 45698;
    strncpy(app_info.md5, "adef12d33564554664545546645bcdef", 32);
    // rc = fal_partition_write(download_partition, 0, (uint8_t *)&app_info, sizeof(app_struct));
    // if (rc < 0)
    // {
    //     log_e("connot find the part download_partition");
    //     return -1;
    // }

    app_info_p = (app_struct_t)(FLASH_BASE + download_partition->offset);
    printf("%-*.*s:0x%04x\r\n", 10, 8, "app_flag", app_info_p->app_flag);
    printf("%-*.*s:%d\r\n", 10, 8, "size", app_info_p->size);
    printf("%-*.*s:%s\r\n", 10, 8, "md5", app_info_p->md5);
    printf("%-*.*s:%s\r\n", 10, 8, "version", app_info_p->version);
    printf("%-*.*s:%s\r\n", 10, 8, "url", app_info_p->url);

    if (app_info_p->app_flag != FLASH_APP_FLAG_WORD)
    {
        log_e("FLASH_APP_FLAG_WORD is correct");
        return -1;
    }
    uint8_t md5[33] = {0};
    _md5((uint8_t *)(FLASH_BASE + download_partition->offset + sizeof(app_struct)), app_info_p->size, md5);
    log_d("md5:%s", md5);

    if (strncasecmp(&app_info_p->md5[0], (char *)md5, 32) != 0)
    {
        log_e("md5 err");
        return -1;
    }

    rc = Erase_partition("app");
    if (rc <= 0)
    {
        log_e("Erase app err");
        return -1;
    }
    log_i("Erase app size:%d", rc);
    rc = fal_partition_write(app_partition, 0, (uint8_t *)(FLASH_BASE + download_partition->offset + sizeof(app_struct)), app_info_p->size);
    if (rc < 0)
    {
        log_e("partition_write failed");
        return -1;
    }
     memset(&app_info,0,sizeof(app_struct)) ;
    rc = fal_partition_write(download_partition, 0, (uint8_t *)(&app_info), sizeof(app_struct));
    if (rc < 0)
    {
        log_e("app_flag clear failed");
        return -1;
    }
    return rc;
}

void _hmac_md5(const char *msg, int msg_len, char *digest, const char *key, int key_len)
{
    HMAC_MD5ctx_stt HMAC_MD5ctx_st;
    uint32_t error_status = HASH_SUCCESS;

    HMAC_MD5ctx_st.mFlags = E_HASH_DEFAULT;
    HMAC_MD5ctx_st.mTagSize = CRL_MD5_SIZE;
    log_d("msg:%s", msg);
    log_d("key:%s", key);
    Crypto_DeInit();
    HMAC_MD5ctx_st.pmKey = (const uint8_t *)(key);
    HMAC_MD5ctx_st.mKeySize = key_len;
    error_status = HMAC_MD5_Init(&HMAC_MD5ctx_st);
    /* check for initialization errors */
    if (error_status == HASH_SUCCESS)
    {
        /* Add data to be hashed */
        error_status = HMAC_MD5_Append(&HMAC_MD5ctx_st, (const uint8_t *)msg, strlen(msg));

        if (error_status == HASH_SUCCESS)
        {
            int32_t P_pOutputSize;
            int8_t OutputBuffer[16];
            memset(digest, 0, 32);
            /* retrieve */
            error_status = HMAC_MD5_Finish(&HMAC_MD5ctx_st, (uint8_t *)OutputBuffer, &P_pOutputSize);
            if (error_status == HASH_SUCCESS)
            {
                printf("\r\nOutputSize:%dmd5:", P_pOutputSize);
                for (int i = 0; i < P_pOutputSize; i++)
                {
                    printf("%02x", OutputBuffer[i]);
                    sprintf(digest + 2 * i, "%02x", OutputBuffer[i]);
                }
                printf("\r\n");
                printf("digest:%s\r\n", digest);
            }
            else
            {
                log_e("HMAC_MD5_Append err:%d", error_status);
            }
        }
        else
        {
            log_e("HMAC_MD5_Finish err:%d", error_status);
        }
    }
    else
    {
        log_e("HMAC_MD5_Init err:%d", error_status);
    }
}
void _md5(const unsigned char *input, size_t ilen, unsigned char output[32])
{
    MD5ctx_stt MD5ctx_st;
    uint32_t error_status = HASH_SUCCESS;

    Crypto_DeInit();
    MD5ctx_st.mFlags = E_HASH_DEFAULT;
    MD5ctx_st.mTagSize = CRL_MD5_SIZE;

    error_status = MD5_Init(&MD5ctx_st);
    if (error_status == HASH_SUCCESS)
    {
        error_status = MD5_Append(&MD5ctx_st, input, ilen);
        if (error_status == HASH_SUCCESS)
        {
            int32_t P_pOutputSize;
            uint8_t OutputBuffer[16];
            error_status = MD5_Finish(&MD5ctx_st, OutputBuffer, &P_pOutputSize);
            if (error_status == HASH_SUCCESS)
            {
                printf("\r\nOutputSize:%dmd5:", P_pOutputSize);
                for (int i = 0; i < P_pOutputSize; i++)
                {
                    printf("%02x", OutputBuffer[i]);
                    sprintf((char *)(output + 2 * i), "%02x", OutputBuffer[i]);
                }
                printf("\r\n");
                printf("%s\r\n", output);
            }
            else
            {
                log_e("MD5_Finish err:%d", error_status);
            }
        }
        else
        {
            log_e("MD5_Append err:%d", error_status);
        }
    }
    else
    {
        log_e("MD5_Init err:%d", error_status);
    }
}

int isthereOTAflag(void)
{
    app_struct_t app_info_p;
    const struct fal_partition *download_partition = NULL;
    fal_init();

    download_partition = fal_partition_find("download");
    if (download_partition == NULL)
        return 0;
    app_info_p = (app_struct_t)(FLASH_BASE + download_partition->offset);
    if (app_info_p->app_flag != FLASH_APP_FLAG_WORD)
        return 0;
    return 1;
}
