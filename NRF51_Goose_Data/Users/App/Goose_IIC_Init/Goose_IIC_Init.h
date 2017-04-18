/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  Goose_IIC_Init.h
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  06/04/2017
* Description        :  Goose IIC Init header file
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GOOSE_IIC_INIT_H
#define __GOOSE_IIC_INIT_H

/* Includes ------------------------------------------------------------------*/
#include "Global.h"

/* Private define ------------------------------------------------------------*/
#define     Goose_TIMEOUT 				(5 * 1000)                      // 超时周期us

//#define 	Goose_IIC_GPIO_RCC			RCC_APB2Periph_GPIOB			// G组管脚
#define 	Goose_IIC_SDA				1								// SDA 19
#define 	Goose_IIC_SCL				2								// SCL 18
//#define 	Goose_GPIO					GPIOB
/* Private typedef -----------------------------------------------------------*/
// Sensor Commands

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern  u8 Goose_IIC_Init(void);                                   		// 管脚初始化

#endif /* __GOOSE_IIC_INIT_H */

/******************* (C) COPYRIGHT 2017 王宇 *****END OF FILE******************/
