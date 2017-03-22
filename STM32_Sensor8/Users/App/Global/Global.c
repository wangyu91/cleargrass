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

/* Private Function Prototypes -----------------------------------------------*/
void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx);

/* Private Functions ---------------------------------------------------------*/
/*******************************************************************************
*						王宇@2017-03-21
* Function Name   :  GPIO_Pin_Config
* Description	  :  GPIO Pin 初始化
* Input			  :  uint_32 *GPIOx		GPIO组
					 uint_16 *Pin		管脚Pin
					 uint_t32 Mode		管脚输出模式
					 uint_16 Speed		管脚速率
* Output          :  None
* Return		  :  None
*******************************************************************************/
    void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx)
    {
        GPIO_InitTypeDef GPIO_Init_s;									// 初始化配置结构体
    
        RCC_APB2PeriphClockCmd(RCC_APB2P_GPIOx, ENABLE);                // 启动GPIO时钟
    
        GPIO_Init_s.GPIO_Pin    = Pin;                                  // 设定管脚
        GPIO_Init_s.GPIO_Mode   = Mode;                                 // 管脚输出模式
        GPIO_Init_s.GPIO_Speed  = Speed;                                // 管脚速率
    
        GPIO_Init(GPIOx, &GPIO_Init_s);                                 // 初始化管脚
    }
    
/******************* (C) COPYRIGHT 2017 王宇 ********END OF FILE***************/
