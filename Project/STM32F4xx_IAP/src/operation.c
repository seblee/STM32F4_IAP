#include "operation.h"
#include "string.h"
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
    app_info.app_len = 45698;
    strncpy(app_info.md5, "adef12d33564554664545546645bcdef", 32);
    // rc = fal_partition_write(download_partition, 0, (uint8_t *)&app_info, sizeof(app_struct));
    // if (rc < 0)
    // {
    //     log_e("connot find the part download_partition");
    //     return -1;
    // }

    app_info_p = (app_struct_t)(FLASH_BASE + download_partition->offset);
    printf("%-*.*s:0x%04x\r\n", 10, 8, "app_flag", app_info_p->app_flag);
    printf("%-*.*s:%d\r\n", 10, 8, "app_len", app_info_p->app_len);
    printf("%-*.*s:", 10, 8, "md5");
    for (int i = 0; i < 32; i++)
    {
        printf("%c", app_info_p->md5[i]);
    }
    printf("\r\n");

    if (app_info_p->app_flag != FLASH_APP_FLAG_WORD)
    {
        log_e("FLASH_APP_FLAG_WORD is not right");
        return -1;
    }
    rc = Erase_partition("app");
    if (rc <= 0)
    {
        log_e("Erase app err");
        return -1;
    }
    log_e("Erase app size:%d", rc);
    rc = fal_partition_write(app_partition, 0, (uint8_t *)(app_info_p + sizeof(app_struct)), app_info_p->app_len);
    if (rc < 0)
    {
        log_e("connot find the part download_partition");
        return -1;
    }
    rc = 0;
    return rc;
}
