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

#include "Sgpc1x.h"
#include "Sensirion_common.h"

/* Private Defines -----------------------------------------------------------*/
#define RED0_PIN			GPIO_Pin_8
#define RED0_GPIO			GPIOG
#define RED1_PIN			GPIO_Pin_9
#define RED1_GPIO			GPIOC
#define RED2_PIN			GPIO_Pin_11
#define RED2_GPIO			GPIOA
#define RED3_PIN			GPIO_Pin_2
#define RED3_GPIO			GPIOD
#define RED4_PIN			GPIO_Pin_13
#define RED4_GPIO			GPIOG
#define RED5_PIN			GPIO_Pin_6
#define RED5_GPIO			GPIOB
#define RED6_PIN			GPIO_Pin_14
#define RED6_GPIO			GPIOC
#define RED7_PIN			GPIO_Pin_2
#define RED7_GPIO			GPIOF

#define IIC8_SHT30														// Debug 测试
#define IIC8_BH1721
#define IIC8_SGPC10

/* Private Variables ---------------------------------------------------------*/
extern SW_IIC_t SHT30_s;												// SHT30 结构体
extern SW_IIC_t Sensor8_IIC_s;											// IIC初始化结构体
extern SW_IIC_t Sensor8_IIC1_s;
extern SW_IIC_t Sensor8_IIC2_s;
extern SW_IIC_t Sensor8_IIC3_s;
extern SW_IIC_t Sensor8_IIC4_s;
extern SW_IIC_t Sensor8_IIC5_s;
extern SW_IIC_t Sensor8_IIC6_s;
extern SW_IIC_t Sensor8_IIC7_s;

extern u16		System_Time_Delay;										// 系统延时变量
/* Private Typedefs ----------------------------------------------------------*/

/* Private Function Prototypes -----------------------------------------------*/
extern void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx);	// 管脚初始化

extern void STM32_Delay_us(uint16_t time_us);							// 粗延时 微秒
extern void STM32_Delay_ms(uint16_t time_ms);							// 毫秒

//static void IIC_Delay(unsigned char count);								// IIC延时

/* __GLOBAL_H */

/******************* (C) COPYRIGHT 2017 王宇 ********END OF FILE***************/
