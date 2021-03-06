/**
  ******************************************************************************
  * @file    STM32F2xx_IAP/src/menu.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    02-May-2011
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

/** @addtogroup STM32F2xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "flash_if.h"
#include "menu.h"
#include "ymodem.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t FlashProtection = 0;
uint8_t tab_1024[1024] =
  {
    0
  };
uint8_t FileName[FILE_NAME_LENGTH];
uint32_t FlashErased = 0;

/* Private function prototypes -----------------------------------------------*/
void SerialDownload(void);
void SerialUpload(void);

/* Private functions ---------------------------------------------------------*/

#if 0
/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
void SerialDownload(void)
{
  uint8_t Number[10] = "          ";
  int32_t Size = 0;

  SerialPutString("Waiting for the file to be sent ... (press 'a' to abort)\n\n\r");
  Size = Ymodem_Receive(&tab_1024[0]);
  if (Size > 0)
  {
    SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
    SerialPutString(FileName);
    Int2Str(Number, Size);
    SerialPutString("\n\r Size: ");
    SerialPutString(Number);
    SerialPutString(" Bytes\r\n");
    SerialPutString("--------------------------------\n");
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
  else if (Size == -4)
  {
    SerialPutString("\r\n\nReceive the file timeout!\n\r");
  }  
  else
  {
    SerialPutString("\n\n\rFailed to receive the file!\n\r");
  }
}
#else
/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
void SerialDownload(void)
{
  uint8_t Number[10] = "          ";
  int32_t Size = 0;

  SerialPutString("Waiting for the file to be sent, within 30 seconds ... (press 'r' to restart)\n\n\r");
  Size = Ymodem_Receive(&tab_1024[0]);
  if (Size > 0)
  {
    SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
    SerialPutString(FileName);
    Int2Str(Number, Size);
    SerialPutString("\n\r Size: ");
    SerialPutString(Number);
    SerialPutString(" Bytes\r\n");
    SerialPutString("--------------------------------\n");
    
    /* Execute the new program */
    FLASH_If_JumpToApplication();
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
    SerialPutString("\r\n\nRestart by user.\n\r");
  }
  else if (Size == -4)
  {
    SerialPutString("\r\n\nReceive the file timeout!\n\r");

    if (0 == FlashErased)
    {
      FLASH_If_JumpToApplication();
    }
  }  
  else
  {
    SerialPutString("\n\n\rFailed to receive the file!\n\r");
  }

  if (1 == FlashErased)
  {
    FLASH_If_ApplicationDeInit();
    
    SerialPutString("Application is broken, please try to update again ...\r\n"); 
  }    
}
#endif

/**
  * @brief  Upload a file via serial port.
  * @param  None
  * @retval None
  */
void SerialUpload(void)
{
  uint8_t status = 0 ; 

  SerialPutString("Select Receive File ... (press any key to abort)\n\n\r");

  if (GetKey() == CRC16)
  {
    /* Transmit the flash image through ymodem protocol */
    status = Ymodem_Transmit((uint8_t*)APPLICATION_ADDRESS, (const uint8_t*)"UploadedFlashImage.bin", USER_FLASH_SIZE);

    if (status != 0) 
    {
      SerialPutString("\n\n\rError Occured while Transmitting File!\n\r");
    }
    else
    {
      SerialPutString("\n\n\rFile Trasmitted Successfully!\n\r");
    }
  }
  else
  {
    SerialPutString("\r\n\nAborted by user.\n\r");  
  }  
}

#if 0
/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
void Main_Menu(void)
{
  uint8_t key = 0;

  SerialPutString("\r\n===================================================================");
  SerialPutString("\r\n=              (C) COPYRIGHT 2011 STMicroelectronics              =");
  SerialPutString("\r\n=                                                                 =");
  SerialPutString("\r\n=     In-Application Programming Application  (Version 1.0.0)     =");
  SerialPutString("\r\n=                                                                 =");
  SerialPutString("\r\n=                                    By MCD Application Team      =");
  SerialPutString("\r\n=                                                                 =");
  SerialPutString("\r\n=                               Modified by 591881218@qq.com      =");
  SerialPutString("\r\n===================================================================");
  SerialPutString("\r\n\r\n");

  /* Test if any sector of Flash memory where user application will be loaded is write protected */
  if (FLASH_If_GetWriteProtectionStatus() == 0)   
  {
    FlashProtection = 1;
  }
  else
  {
    FlashProtection = 0;
  }

  while (1)
  {
    SerialPutString("\r\n======================= Main Menu =======================\r\n\n");
    SerialPutString(" Download Image To the STM32F2xx Internal Flash ------ 1\r\n\n");
    SerialPutString(" Upload Image From the STM32F2xx Internal Flash ------ 2\r\n\n");
    SerialPutString(" Execute The New Program ----------------------------- 3\r\n\n");

    if(FlashProtection != 0)
    {
      SerialPutString(" Disable the write protection ------------------------ 4\r\n\n");
    }

    SerialPutString("=========================================================\r\n\n");

    /* Receive key */
    key = GetKey();

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
      FLASH_If_JumpToApplication();
    }
    else if ((key == 0x34) && (FlashProtection == 1))
    {
      /* Disable the write protection */
      switch (FLASH_If_DisableWriteProtection())
      {
        case 1:
        {
          SerialPutString("Write Protection disabled ...\r\n");
          FlashProtection = 0;
          break;
        }
        case 2:
        {
          SerialPutString("Error: Flash write unprotection failed ...\r\n");
          break;
        }
        default:
        {
        }
      }
    }
    else
    {
      if (FlashProtection == 0)
      {
        SerialPutString("Invalid Number! ==> The number should be either 1, 2 or 3\r\n");
      }
      else
      {
        SerialPutString("Invalid Number! ==> The number should be either 1, 2, 3 or 4\r\n");
      }
    }
  }
}
#else
/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
void Main_Menu(void)
{
  /* Test if any sector of Flash memory where user application will be loaded is write protected */
  if (FLASH_If_GetWriteProtectionStatus() == 0)   
  {
    FlashProtection = 1;
  }
  else
  {
    FlashProtection = 0;
  }

  while (1)
  {
    if (FlashProtection != 0)
    {
      /* Disable the write protection */
      switch (FLASH_If_DisableWriteProtection())
      {
        case 1:
        {
          SerialPutString("Write Protection disabled ...\r\n");
          FlashProtection = 0;
          break;
        }
        case 2:
        {
          SerialPutString("Error: Flash write unprotection failed ...\r\n");
          
          if (1 == FlashErased)
          {
            FLASH_If_ApplicationDeInit();
            
            SerialPutString("Application is broken, please try to update again ...\r\n"); 
          }   
          else
          {
            FLASH_If_JumpToApplication();
          }

          break;
        }
        default:
        {
          SerialPutString("Flash memory where user application will be loaded is write protected ...\r\n");  

          if (1 == FlashErased)
          {
            FLASH_If_ApplicationDeInit();
            
            SerialPutString("Application is broken, please try to update again ...\r\n"); 
          }   
          else
          {
            FLASH_If_JumpToApplication();
          }

          break;
        }
      }        
    }
    else
    {
      /* Download user application in the Flash */
      SerialDownload();
    }
  }
}
#endif

/**
  * @}
  */

/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE******/
