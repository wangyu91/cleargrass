/******************** (C) COPYRIGHT 2016 ���� **********************************
* File Name          :  Goose_IIC_Init.c
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  06/04/2017
* Description        :  Goose IIC ��ʼ��
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Goose_IIC_Init.h"
#include "SW_IIC.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Goose_IIC_Delay_us(void);											// us��ʱ
void Goose_IIC_Pin_Init(void);                                      	// �ܽų�ʼ��
void Goose_IIC_Set_SDA_High(void);                                      // ����������
void Goose_IIC_Set_SDA_Low(void);                                       // ����������
void Goose_IIC_Set_SCL_High(void);                                      // ����ʱ��
void Goose_IIC_Set_SCL_Low(void);                                       // ����ʱ��
void Goose_IIC_Set_SDA_Input(void);                                  	// ����SDAΪ����ģʽ
void Goose_IIC_Set_SDA_Output(void);                                 	// ����SDAΪ���ģʽ
//void Goose_IIC_Set_SCL_Output(void);                               	// ����SCLΪ���ģʽ
u8   Goose_IIC_SDA_Read(void);                                      	// ��ȡSDA��ƽ
u8   Goose_IIC_SCL_Read(void);                                       	// ��ȡSCL��ƽ

u8   Goose_IIC_Init(void);                                   			// Goose IIC�˿ڳ�ʼ��    
/* Private functions ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
//*                           ����@2017-03-21
//* Function Name  :  Goose_IIC_Delay
//* Description    :  IIC ��ʱ
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//void Goose_IIC_Delay(void)
//{
//    nrf_delay_us(4);
//    
//}// End of void Goose_IIC_Delay(void)

/*******************************************************************************
*                           ����@2017-03-27
* Function Name  :  Goose_IIC_Delay_us
* Description    :  IIC us��ʱ
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Delay_us(void)
{
    STM32_Delay_us(1);
    
}
// End of void Goose_IIC_Delay_us(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Goose_IIC_Pin_Init
* Description    :  �ܽų�ʼ��                   
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Pin_Init(void)
{
    
    // ���ùܽ�Ϊ��©ģʽ
    GPIO_Pin_Config(Goose_IIC_GPIO_RCC, Goose_IIC_SDA|Goose_IIC_SCL, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, Goose_GPIO);   // SDA|A5�� SCL|A4�� ��©��� ����50MHz 
   // GPIO_Pin_Config(Goose_IIC_GPIO_C, Goose_IIC_SCL, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, Goose_IIC_GPIO);	// SCL A6��
}
// End of u8  Goose_IIC_Pin_Init(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Goose_IIC_SDA_High
* Description    :  Goose IIC����SDA
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Set_SDA_High(void)
{
    GPIO_SetBits(Goose_GPIO, Goose_IIC_SDA);
    
}
// End of void Goose_IIC_SDA_High(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Goose_IIC_SDA_Low
* Description    :  Goose IIC����SDA
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Set_SDA_Low(void)
{
    GPIO_ResetBits(Goose_GPIO, Goose_IIC_SDA);
    
}
// End of void Goose_IIC_SDA_Low(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Goose_IIC_SCL_High
* Description    :  Goose IIC����SCL
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Set_SCL_High(void)
{
    GPIO_SetBits(Goose_GPIO, Goose_IIC_SCL);
    
}
// End of void Goose_IIC_SCL_High(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Goose_IIC_SCL_Low
* Description    :  Goose IIC����SCL
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Set_SCL_Low(void)
{
    GPIO_ResetBits(Goose_GPIO, Goose_IIC_SCL);
    
}
// End of void Goose_IIC_SCL_Low(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Goose_IIC_Set_SDA_Input
* Description    :  ����SDA�ܽ�Ϊ����ģʽ
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Set_SDA_Input(void)
{
    GPIO_SetBits(Goose_GPIO, Goose_IIC_SDA);
}
// End of void Goose_IIC_Set_SDA_Input(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Goose_IIC_Set_SDA_Output
* Description    :  ����SDA�ܽ�Ϊ���ģʽ
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Set_SDA_Output(void)
{
   	 GPIO_ResetBits(Goose_GPIO, Goose_IIC_SDA);
}
// End of void Goose_IIC_Set_SDA_Output(void)

/*******************************************************************************
//*                           ����@2017-03-21
//* Function Name  :  Goose_IIC_Set_SCL_Output
//* Description    :  ����SCL�ܽ�Ϊ���ģʽ
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//void Goose_IIC_Set_SCL_Output(void)
//{
//    NRF_GPIO->DIRSET = (1UL << Goose_IIC_SCL_PIN);
//    
//}// End of void Goose_IIC_Set_SCL_Output(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Goose_IIC_SDA_Read
* Description    :  ��ȡSDA�ܽŵ�ƽ
* Input          :  None
* Output         :  None
* Return         :  �ߵ�ƽ����1 �͵�ƽ����0
*******************************************************************************/
u8 Goose_IIC_SDA_Read(void)
{
    return(GPIO_ReadInputDataBit(Goose_GPIO, Goose_IIC_SDA));
    
}
// End of u8 Goose_IIC_SDA_Read(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Goose_IIC_SCL_Read
* Description    :  ��ȡSCL�ܽŵ�ƽ
* Input          :  None
* Output         :  None
* Return         :  �ߵ�ƽ����1 �͵�ƽ����0
*******************************************************************************/
u8 Goose_IIC_SCL_Read(void)
{
    return(GPIO_ReadInputDataBit(Goose_GPIO, Goose_IIC_SCL));
    
}
// End of u8 Goose_IIC_SCL_Read(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Goose_IIC_Port_Init
* Description    :  Goose IIC�˿ڳ�ʼ��
* Input          :  None
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 Goose_IIC_Init(void)
{
    u8 Transfer_Succeeded; 

	Goose_IIC_Pin_Init();												// ��ʼ���ܽ�
	
	// ���ýṹ��
    Goose_IIC_s.Delay_us              = Goose_IIC_Delay_us;
    Goose_IIC_s.SDA_High              = Goose_IIC_Set_SDA_High;
    Goose_IIC_s.SDA_Low               = Goose_IIC_Set_SDA_Low;
    Goose_IIC_s.SCL_High              = Goose_IIC_Set_SCL_High;
    Goose_IIC_s.SCL_Low               = Goose_IIC_Set_SCL_Low;
    Goose_IIC_s.Set_SDA_Input         = Goose_IIC_Set_SDA_Input; 
    Goose_IIC_s.Set_SDA_Output        = Goose_IIC_Set_SDA_Output; 
//    Goose_IIC_s.Set_SCL_Output        = Goose_IIC_Set_SCL_Output;   
    Goose_IIC_s.SDA_Read              = Goose_IIC_SDA_Read; 
    Goose_IIC_s.SCL_Read              = Goose_IIC_SCL_Read;
    Goose_IIC_s.Wait_Scl_Free_Timeout = Goose_TIMEOUT;
    
    Transfer_Succeeded = SW_IIC_Init(&Goose_IIC_s);						// ��ʼ��IICͨ��
    
    return (Transfer_Succeeded);
       
}
// End of u8  Goose_IIC_Port_Init(void)

/******************* (C) COPYRIGHT 2017 ���� **************END OF FILE*********/
