/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  Global.h
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  21/03/2017
* Description        :  Global header file
*******************************************************************************/
/* Ifndef --------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "SW_IIC.h"
#include "SHT30_Init.h"
#include "SHT30_Driver.h"
//#include "GPIO_LED.h"

/* Private Defines -----------------------------------------------------------*/

/* Private Variables ---------------------------------------------------------*/
extern SW_IIC_t LED_t;
extern SW_IIC_t SHT30_s;												// SHT30 结构体

/* Private Typedefs ----------------------------------------------------------*/

/* Private Function Prototypes -----------------------------------------------*/
extern void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx);	// 管脚初始化

extern void STM32_Delay_us(uint16_t time_us);							// 粗延时 微秒
extern void STM32_Delay_ms(uint16_t time_ms);							// 毫秒

/* __GLOBAL_H */

/******************* (C) COPYRIGHT 2017 王宇 ********END OF FILE***************/
