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
SW_IIC_t LED_t;
SW_IIC_t SHT30_s;														// SHT30 �ṹ��
SW_IIC_t Sensor8_IIC_s;													// IIC��ʼ���ṹ��
SW_IIC_t Sensor8_IIC1_s;
SW_IIC_t Sensor8_IIC2_s;
SW_IIC_t Sensor8_IIC3_s;
SW_IIC_t Sensor8_IIC4_s;
SW_IIC_t Sensor8_IIC5_s;
SW_IIC_t Sensor8_IIC6_s;
SW_IIC_t Sensor8_IIC7_s;

/* Private Function Prototypes -----------------------------------------------*/
void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx);	// GPIO�ڳ�ʼ��

void STM32_Delay_us(uint16_t time_us);									// ����ʱ ΢��
void STM32_Delay_ms(uint16_t time_ms);									// ����

//static void IIC_Delay(unsigned char count);								// ��ʱ

/* Private Functions ---------------------------------------------------------*/
/*******************************************************************************
*						����@2017-03-21
* Function Name   :  GPIO_Pin_Config
* Description	  :  GPIO Pin ��ʼ��
* Input			  :  uint32_t 		RCC_APB2P_GPIOx		ʼ�ճ�ʼ���ܽ���
					 uint16_t 		Pin					�ܽ�Pin
					 uint32_t 		Mode				�ܽ����ģʽ
					 uint32_t 		Speed				�ܽ�����
					 GPIO_TypeDef * GPIOx				�ܽ���
* Output          :  None
* Return		  :  None
*******************************************************************************/
void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx)
{
    GPIO_InitTypeDef GPIO_Init_s;										// ��ʼ�����ýṹ��
    
    RCC_APB2PeriphClockCmd(RCC_APB2P_GPIOx, ENABLE);                	// ����GPIOʱ��
    
    GPIO_Init_s.GPIO_Pin    = Pin;                                	    // �趨�ܽ�
    GPIO_Init_s.GPIO_Mode   = Mode;                               	    // �ܽ����ģʽ
    GPIO_Init_s.GPIO_Speed  = Speed;                               		// �ܽ�����
    
	GPIO_Init(GPIOx, &GPIO_Init_s);										// ��ʼ���ܽ�
}
// end of  void GPIO_Pin_Config(uint32_t RCC_APB2P_GPIOx, uint16_t Pin, uint32_t Mode, uint16_t Speed, GPIO_TypeDef *GPIOx)

/*******************************************************************************
*						����@2017-03-23
* Function Name   :  STM32_Delay_us
* Description	  :  ����ʱ ΢��
* Input			  :  uint16_t time_us	��ʱֵ
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
*						����@2017-03-23
* Function Name   :  STM32_Delay_ms
* Description	  :  ����ʱ ����
* Input			  :  uint16_t time_ms	��ʱֵ
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
//*                           ����@2017-03-22
//* Function Name  :  IIC_Delay
//* Description    :  IIC ʱ��
//* Input          :  unsigned char count     ʱ�Ӽ���
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

/******************* (C) COPYRIGHT 2017 ���� ********END OF FILE***************/
