/**
  ******************************************************************************
  * @file    STM32F4xx_IAP/src/menu.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    10-October-2011
  * @brief   This file provides the software which contains the main menu routine.
  *          The main menu gives the options of:
  *             - downloading a new binary file, 
  *             - uploading internal flash memory,
  *             - executing the binary file already loaded 
  *             - disabling the write protection of the Flash sectors where the 
  *               user loads his binary file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/** @addtogroup STM32F4xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "flash_if.h"
#include "menu.h"
#include "ymodem.h"
#include <stdio.h>
#include "fal.h"
#include "operation.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction Jump_To_Application;
uint32_t JumpAddress;
__IO uint32_t FlashProtection = 0;
uint8_t tab_1024[1024] = {0};
uint8_t FileName[FILE_NAME_LENGTH];

/* Private function prototypes -----------------------------------------------*/
void SerialDownload(void);
void SerialUpload(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
void SerialDownload(void)
{
    uint8_t Number[10] = "          ";
    int32_t Size = 0;

    SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
    Size = Ymodem_Receive(&tab_1024[0]);
    if (Size > 0)
    {
        SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
        SerialPutString(FileName);
        Int2Str(Number, Size);
        SerialPutString("\n\r Size: ");
        SerialPutString(Number);
        SerialPutString(" Bytes\r\n");
        SerialPutString("-------------------\n");
    }
    else if (Size == -1)
    {
        SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
    }
    else if (Size == -2)
    {
        SerialPutString("\n\n\rVerification failed!\n\r");
    }
    else if (Size == -3)
    {
        SerialPutString("\r\n\nAborted by user.\n\r");
    }
    else
    {
        SerialPutString("\n\rFailed to receive the file!\n\r");
    }
}

/**
  * @brief  Upload a file via serial port.
  * @param  None
  * @retval None
  */
void SerialUpload(void)
{
    uint8_t status = 0;

    SerialPutString("\n\n\rSelect Receive File\n\r");

    if (GetKey() == CRC16)
    {
        /* Transmit the flash image through ymodem protocol */
        status = Ymodem_Transmit((uint8_t *)APPLICATION_ADDRESS, (const uint8_t *)"UploadedFlashImage.bin", USER_FLASH_SIZE);

        if (status != 0)
        {
            SerialPutString("\n\rError Occurred while Transmitting File\n\r");
        }
        else
        {
            SerialPutString("\n\rFile uploaded successfully \n\r");
        }
    }
}

/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
void Main_Menu(void)
{
    uint8_t key = 0;
    int rc;
    RCC_ClocksTypeDef RCC_Clocks;

    fal_init();
    SystemCoreClockUpdate();
    RCC_GetClocksFreq(&RCC_Clocks);
    printf("\r\n");
    printf("%-*.*s:%d\r\n", 27, 27, "SystemCoreClock", SystemCoreClock);
    printf("%-*.*s:%d\r\n", 27, 27, "RCC_Clocks.SYSCLK_Frequency", RCC_Clocks.SYSCLK_Frequency);
    printf("%-*.*s:%d\r\n", 27, 27, "RCC_Clocks.HCLK_Frequency", RCC_Clocks.HCLK_Frequency);
    printf("%-*.*s:%d\r\n", 27, 27, "RCC_Clocks.PCLK1_Frequency", RCC_Clocks.PCLK1_Frequency);
    printf("%-*.*s:%d\r\n", 27, 27, "RCC_Clocks.PCLK2_Frequency", RCC_Clocks.PCLK2_Frequency);

    SerialPutString("\r\n======================================================================");
    SerialPutString("\r\n=              (C) COPYRIGHT 2011 STMicroelectronics                 =");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n=  STM32F4xx In-Application Programming Application  (Version 1.0.0) =");
    SerialPutString("\r\n=                                                                    =");
    SerialPutString("\r\n=                                   By MCD Application Team          =");
    SerialPutString("\r\n======================================================================");
    SerialPutString("\r\n\r\n");

    /* Test if any sector of Flash memory where user application will be loaded is write protected */
    if (FLASH_If_GetWriteProtectionStatus() == 0)
        FlashProtection = 1;
    else
        FlashProtection = 0;

    while (1)
    {
        SerialPutString("\r\n================== Main Menu ============================\r\n");
        SerialPutString("  Download Image To the STM32F4xx Internal Flash ------ 1\r\n");
        SerialPutString("  Upload Image From the STM32F4xx Internal Flash ------ 2\r\n");
        SerialPutString("  Execute The New Program ----------------------------- 3\r\n");
        if (FlashProtection != 0)
        {
            SerialPutString("  Disable the write protection ------------------------ 4\r\n");
        }
        SerialPutString("  Erase parameter partition --------------------------- 5\r\n");
        SerialPutString("  Erase app partition --------------------------------- 6\r\n");
        SerialPutString("  Erase download partition ---------------------------- 7\r\n");
        SerialPutString("  copy app from download partition to app partition --- 8\r\n");

        SerialPutString("=========================================================\r\n");

        /* Receive key */
        key = GetKey();
        printf("key_value:0x%02x\r\n\n", key);

        if (key == 0x31)
        {
            /* Download user application in the Flash */
            SerialDownload();
        }
        else if (key == 0x32)
        {
            /* Upload user application from the Flash */
            SerialUpload();
        }
        else if (key == 0x33) /* execute the new program */
        {
            JumpAddress = *(__IO uint32_t *)(APPLICATION_ADDRESS + 4);
            /* Jump to user application */
            Jump_To_Application = (pFunction)JumpAddress;
            /* Initialize user application's Stack Pointer */
            __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS);
            Jump_To_Application();
        }
        else if ((key == 0x34) && (FlashProtection == 1))
        {
            /* Disable the write protection */
            switch (FLASH_If_DisableWriteProtection())
            {
            case 1:
            {
                SerialPutString("Write Protection disabled...\r\n");
                FlashProtection = 0;
                break;
            }
            case 2:
            {
                SerialPutString("Error: Flash write unprotection failed...\r\n");
                break;
            }
            default:
                break;
            }
        }
        else if (key == 0x35)
        {
            rc = Erase_partition("parameter");
            if (rc <= 0)
            {
                log_e("Erase parameter err");
            }
            else
            {
                log_e("Erase parameter size:%d", rc);
            }
        }
        else if (key == 0x36)
        {
            rc = Erase_partition("app");
            if (rc <= 0)
            {
                log_e("Erase app err");
            }
            else
            {
                log_e("Erase app size:%d", rc);
            }
        }
        else if (key == 0x37)
        {
            rc = Erase_partition("download");
            if (rc <= 0)
            {
                log_e("Erase download err");
            }
            else
            {
                log_e("Erase download size:%d", rc);
            }
        }
        else if (key == 0x38)
        {
            if (Copy_program() == 0)
            {
                log_e("Copy_program OK");
            }
            else
            {
                log_e("Copy_program err");
            }
        }
        else
        {
            if (FlashProtection == 0)
            {
                SerialPutString("Invalid Number ! ==> The number should be either 1, 2 or 3\r");
            }
            else
            {
                SerialPutString("Invalid Number ! ==> The number should be either 1, 2, 3 or 4\r");
            }
        }
    }
}

/**
  * @}
  */

/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE******/
