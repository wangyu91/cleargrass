/******************** (C) COPYRIGHT 2017 Õı”Ó **********************************
* File Name          :  KH25_Flash_Store.h
* Author             :  Õı”Ó
* CPU Type           :  nRF51
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  14/04/2017
* Description        :  KH25_Flash_Store header file
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KH25_FLASH_STORE_H
#define __KH25_FLASH_STORE_H

/* Includes ------------------------------------------------------------------*/
#include "Global.h"

/* Private define ------------------------------------------------------------*/
#define		Page_Index		0x01FF

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Store_History_Data(void);
void Get_Current_Data(u8* Data_Buff);

#endif /* __KH25_FLASH_STORE_H */

/******************* (C) COPYRIGHT 2017 Õı”Ó *****END OF FILE******************/

