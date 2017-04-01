/******************** (C) COPYRIGHT 2016 王宇 **********************************
* File Name          :  Global.c
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  21/03/2017
* Description        :  全局定义
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Global.h"
/* Private Variables ---------------------------------------------------------*/
SW_IIC_t LED_t;
SW_IIC_t SHT30_s;														// SHT30 结构体
SW_IIC_t Sensor8_IIC_s;													// IIC初始化结构体
SW_IIC_t Sensor8_IIC1_s;
SW_IIC_t Sensor8_IIC2_s;
SW_IIC_t Sensor8_IIC3_s;
SW_IIC_t Sensor8_IIC4_s;
SW_IIC_t Sensor8_IIC5_s;
SW_IIC_t Sensor8_IIC6_s;
SW_IIC_t Sensor8_IIC7_s;

/* Private Function Prototypes -----------------------------------------------*/
void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx);	// GPIO口初始化

void STM32_Delay_us(uint16_t time_us);									// 粗延时 微秒
void STM32_Delay_ms(uint16_t time_ms);									// 毫秒

//static void IIC_Delay(unsigned char count);								// 延时

/* Private Functions ---------------------------------------------------------*/
/*******************************************************************************
*						王宇@2017-03-21
* Function Name   :  GPIO_Pin_Config
* Description	  :  GPIO Pin 初始化
* Input			  :  uint32_t 		RCC_APB2P_GPIOx		始终初始化管脚组
					 uint16_t 		Pin					管脚Pin
					 uint32_t 		Mode				管脚输出模式
					 uint32_t 		Speed				管脚速率
					 GPIO_TypeDef * GPIOx				管脚组
* Output          :  None
* Return		  :  None
*******************************************************************************/
void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx)
{
    GPIO_InitTypeDef GPIO_Init_s;										// 初始化配置结构体
    
    RCC_APB2PeriphClockCmd(RCC_APB2P_GPIOx, ENABLE);                	// 启动GPIO时钟
    
    GPIO_Init_s.GPIO_Pin    = Pin;                                	    // 设定管脚
    GPIO_Init_s.GPIO_Mode   = Mode;                               	    // 管脚输出模式
    GPIO_Init_s.GPIO_Speed  = Speed;                               		// 管脚速率
    
	GPIO_Init(GPIOx, &GPIO_Init_s);										// 初始化管脚
}
// end of  void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx)

/*******************************************************************************
*						王宇@2017-03-23
* Function Name   :  STM32_Delay_us
* Description	  :  粗延时 微秒
* Input			  :  uint16_t time_us	延时值
* Output          :  None
* Return		  :  None
*******************************************************************************/
void STM32_Delay_us(uint16_t time_us)
{
	u16 i = 0;

	while(time_us--)
	{
		i = 10;
		while(i--);
	}
}
// end of void STM32_Delay_us(uint16_t time_us)

/*******************************************************************************
*						王宇@2017-03-23
* Function Name   :  STM32_Delay_ms
* Description	  :  粗延时 毫秒
* Input			  :  uint16_t time_ms	延时值
* Output          :  None
* Return		  :  None
*******************************************************************************/
void STM32_Delay_ms(uint16_t time_ms)
{
	u16 i = 0;

	while(time_ms--)
	{
		i = 12000;
		while(i--);
	}
}
// end of void STM32_Delay_ms(uint16_t time_ms)

/*******************************************************************************
//*                           王宇@2017-03-22
//* Function Name  :  IIC_Delay
//* Description    :  IIC 时延
//* Input          :  unsigned char count     时延计数
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//static void IIC_Delay(unsigned char count)
//{
//	unsigned char i, j;

//	for (i = 0; i < count; i++)
//	{
//		for (j = 0; j < IIC_DELAY_COUNT; j++);
//	}
//}
// end of static void IIC_Delay(unsigned char count)

/******************* (C) COPYRIGHT 2017 王宇 ********END OF FILE***************/
