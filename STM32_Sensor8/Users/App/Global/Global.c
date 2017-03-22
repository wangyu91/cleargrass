/******************** (C) COPYRIGHT 2016 ���� **********************************
* File Name          :  Global.c
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  21/03/2017
* Description        :  ȫ�ֶ���
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Global.h"
/* Private Variables ---------------------------------------------------------*/

/* Private Function Prototypes -----------------------------------------------*/
void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx);

/* Private Functions ---------------------------------------------------------*/
/*******************************************************************************
*						����@2017-03-21
* Function Name   :  GPIO_Pin_Config
* Description	  :  GPIO Pin ��ʼ��
* Input			  :  uint_32 *GPIOx		GPIO��
					 uint_16 *Pin		�ܽ�Pin
					 uint_t32 Mode		�ܽ����ģʽ
					 uint_16 Speed		�ܽ�����
* Output          :  None
* Return		  :  None
*******************************************************************************/
    void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx)
    {
        GPIO_InitTypeDef GPIO_Init_s;									// ��ʼ�����ýṹ��
    
        RCC_APB2PeriphClockCmd(RCC_APB2P_GPIOx, ENABLE);                // ����GPIOʱ��
    
        GPIO_Init_s.GPIO_Pin    = Pin;                                  // �趨�ܽ�
        GPIO_Init_s.GPIO_Mode   = Mode;                                 // �ܽ����ģʽ
        GPIO_Init_s.GPIO_Speed  = Speed;                                // �ܽ�����
    
        GPIO_Init(GPIOx, &GPIO_Init_s);                                 // ��ʼ���ܽ�
    }
    
/******************* (C) COPYRIGHT 2017 ���� ********END OF FILE***************/
