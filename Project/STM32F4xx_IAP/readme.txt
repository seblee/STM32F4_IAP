/**
  @page STM32F4xx_IAP STM32F4xx in-application programming using the USART Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    STM32F4xx_IAP/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    10-October-2011
  * @brief   Description of the "STM32F4xx in-application programming (IAP) using the USART".
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files and pre-configured projects that 
build the In-Application Programming (IAP, through USART) porgram.


@par Directory contents 

 - "STM32F4xx_IAP\inc": contains the IAP firmware header files 
    - STM32F4xx_IAP/inc/common.h            This file provides all the headers of the common functions.
    - STM32F4xx_IAP/inc/flash_if.h          This file provides all the firmware 
                                            function headers of the flash_if.c file.
    - STM32F4xx_IAP/inc/menu.h              This file provides all the firmware
                                            function headers of the menu.c file.
    - STM32F4xx_IAP/inc/ymodem.h            This file provides all the firmware
                                            function headers of the ymodem.c file.
    - STM32F4xx_IAP/inc/stm32f4xx_conf.h    Library Configuration file
    - STM32F4xx_IAP/inc/stm32f4xx_it.h      Header for stm32f4xx_it.c
 - "STM32F4xx_IAP\src": contains the IAP firmware source files
    - STM32F4xx_IAP/src/main.c              Main program
    - STM32F4xx_IAP/src/stm32f4xx_it.c      Interrupt handlers
    - STM32F4xx_IAP/src/flash_if.c          This file provides all the memory related
                                            operation functions(write, erase and
                                            write protection disable)
    - STM32F4xx_IAP/src/menu.c              This file contains the menu to select
                                            downloading a new binary file, uploading
                                            internal Flash memory, executing the binary
                                            and disabling the write protection of
                                            write protected sectors.
    - STM32F4xx_IAP/src/common.c            This file provides functions related to
                                            read/write from/to USART peripheral.
    - STM32F4xx_IAP/src/ymodem.c            This file provides all the firmware functions
                                            related to the ymodem protocol.
    - STM32F4xx_IAP/src/system_stm32f4xx.c  STM32F4xx system source file.
 - "STM32F4xx_IAP\binary_template": contains the binary template firmware source files
 - "STM32F4xx_IAP\EWARM": contains preconfigured project for EWARM toolchain
 - "STM32F4xx_IAP\MDK-ARM": contains preconfigured project for MDK-ARM toolchain
 - "STM32F4xx_IAP\RIDE": contains preconfigured project for RIDE toolchain
 - "STM32F4xx_IAP\TASKING": contains preconfigured project for TASKING toolchain
 - "STM32F4xx_IAP\TrueSTUDIO": contains preconfigured project for TrueSTUDIO toolchain
 

@par Hardware and Software environment

  - This example runs on STM32F4xx Devices.
  
  - This example has been tested with STM324xG-EVAL RevB and can be easily 
    tailored to any other development board.
    
  - STM324xG-EVAL Set-up
    - Connect a null-modem female/female RS232 cable between the DB9 connector 
      CN16 and PC serial port(make sure that jumper JP22 is in position 1-2).
    - Use the Key push-button.
    
  - Hyperterminal configuration:
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: None 


@par How to use it ? 

In order to make the program work, you must do the following:

  1. Generate a binary image using the preconfigured project provided in the directory
      "Project\STM32F4xx_IAP\binary_template" . 
  2. Program the internal Flash with the IAP (see below) 
  3. Open HyperTerminal window using the settings already defined in section
     "Hardware and Software environment" 
  4. To run the IAP application, keep the Key push-button pressed at Reset. 
     The IAP main menu is then displayed on the HyperTerminal window.
  5. To download an application, press 1 and use the Ymodem protocol

  In order to load the IAP code, you have do the following:
   - EWARM
      - Open the Project.eww workspace
      - Rebuild all files: Project->Rebuild all
      - Load project image: Project->Debug
      - Run program: Debug->Go(F5)

   - RIDE
      - Open the Project.rprj project
      - Rebuild all files: Project->build project
      - Load project image: Debug->start(ctrl+D)
      - Run program: Debug->Run(ctrl+F9)

   - MDK-ARM
      - Open the Project.uvproj project
      - Rebuild all files: Project->Rebuild all target files
      - Load project image: Debug->Start/Stop Debug Session
      - Run program: Debug->Run (F5)

   - TASKING
      - Open TASKING toolchain.
      - Click on File->Import, select General->'Existing Projects into Workspace' 
        and then click "Next". 
      - Browse to TASKING workspace directory and select the project: 
          - STM324xG-EVAL: to configure the project for STM32F4xx devices.
      - Under Windows->Preferences->General->Workspace->Linked Resources, add 
        a variable path named "Cur_Path" which points to the folder containing
        "Libraries", "Project" and "Utilities" folders.
      - Rebuild all project files: Select the project in the "Project explorer" 
        window then click on Project->build project menu.
      - Run program: Select the project in the "Project explorer" window then click 
        Run->Debug (F11)

   - TrueSTUDO
      - Open the TrueSTUDIO toolchain.
      - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
      - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
      - Browse to the TrueSTUDIO workspace directory, select the project:
          - STM324xG-EVAL: to configure the project for STM32F4xx devices
      - Under Windows->Preferences->General->Workspace->Linked Resources, add 
        a variable path named "CurPath" which points to the folder containing
        "Libraries", "Project" and "Utilities" folders.
      - Rebuild all project files: Select the project in the "Project explorer" 
        window then click on Project->build project menu.
      - Run program: Run->Debug (F11)

 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 */
