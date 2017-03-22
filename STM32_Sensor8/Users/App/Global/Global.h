/******************** (C) COPYRIGHT 2017 Õı”Ó **********************************
* File Name          :  Global.h
* Author             :  Õı”Ó
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  21/03/2017
* Description        :  Global header file
*******************************************************************************/
/* Ifndef --------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private Defines -----------------------------------------------------------*/

/* Private Variables ---------------------------------------------------------*/

/* Private Typedefs ----------------------------------------------------------*/

/* Private Function Prototypes -----------------------------------------------*/
extern void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx);

/* __GLOBAL_H */

/******************* (C) COPYRIGHT 2017 Õı”Ó ********END OF FILE***************/
