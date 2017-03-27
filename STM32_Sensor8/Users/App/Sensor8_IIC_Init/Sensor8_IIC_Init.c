/******************** (C) COPYRIGHT 2016 ���� **********************************
* File Name          :  Sensor8_IIC_Init.c
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  27/03/2017
* Description        :  һ�ϰ�IIC��ʼ��
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Sensor8_IIC_Init.h"
#include "SW_IIC.h"


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
//void Sensor8_IIC_Delay(void);                                            	// ��ʱ
void Sensor8_IIC_Delay_us(void);                                          	// us��ʱ
void Sensor8_IIC_Pin_Init(void);                                          	// �ܽų�ʼ��
void Sensor8_IIC_Set_SDA_High(void);                                          // ����������
void Sensor8_IIC_Set_SDA_Low(void);                                           // ����������
void Sensor8_IIC_Set_SCL_High(void);                                          // ����ʱ��
void Sensor8_IIC_Set_SCL_Low(void);                                           // ����ʱ��
void Sensor8_IIC_Set_SDA_Input(void);                                     	// ����SDAΪ����ģʽ
void Sensor8_IIC_Set_SDA_Output(void);                                    	// ����SDAΪ���ģʽ
//void Sensor8_IIC_Set_SCL_Output(void);                                    	// ����SCLΪ���ģʽ
u8   Sensor8_IIC_SDA_Read(void);                                          	// ��ȡSDA��ƽ
u8   Sensor8_IIC_SCL_Read(void);                                          	// ��ȡSCL��ƽ

void Sensor8_IIC_Variable_Init(void);                                     	// SHT30������ʼ��
u8   Sensor8_IIC_Init(void);                                         			// SHT30�˿ڳ�ʼ��    
u8 Sensor8_IIC_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u16 usRead_Addr, u8* pBuffer, u16 usRead_Len);// ���Ĵ���
u8 Sensor8_IIC_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len);// д�Ĵ���
/* Private functions ---------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
//*                           ����@2017-03-21
//* Function Name  :  Sensor8_IIC_Delay
//* Description    :  SHT30��ʱ
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//void Sensor8_IIC_Delay(void)
//{
//    nrf_delay_us(4);
//    
//}// End of void Sensor8_IIC_Delay(void)

/*******************************************************************************
*                           ����@2017-03-27
* Function Name  :  Sensor8_IIC_Delay_us
* Description    :  IIC us��ʱ
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sensor8_IIC_Delay_us(void)
{
    STM32_Delay_us(1);
    
}
// End of void Sensor8_IIC_Delay_us(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_Pin_Init
* Description    :  �ܽų�ʼ��                   
* Input          :  P0.12   -->   Sensor8_IIC_SCL_PIN
*                   P0.13   -->   Sensor8_IIC_SDA_PIN
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
void Sensor8_IIC_Pin_Init(void)
{
    
    // ���ùܽ�Ϊ��©ģʽ
    GPIO_Pin_Config(Sensor8_IIC_GPIO_RCC, Sensor8_IIC_SDA|Sensor8_IIC_SCL, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, Sensor8_GPIO);   // SDA|A5�� SCL|A4�� ��©��� ����50MHz 
   // GPIO_Pin_Config(Sensor8_IIC_GPIO_C, Sensor8_IIC_SCL, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, Sensor8_IIC_GPIO);	// SCL A6��
}
// End of u8  Sensor8_IIC_Pin_Init(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_SDA_High
* Description    :  SHT30����SDA
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sensor8_IIC_Set_SDA_High(void)
{
    GPIO_SetBits(Sensor8_GPIO, Sensor8_IIC_SDA);
    
}
// End of void Sensor8_IIC_SDA_High(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_SDA_Low
* Description    :  SHT30����SDA
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sensor8_IIC_Set_SDA_Low(void)
{
    GPIO_ResetBits(Sensor8_GPIO, Sensor8_IIC_SDA);
    
}
// End of void Sensor8_IIC_SDA_Low(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_SCL_High
* Description    :  SHT30����SCL
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sensor8_IIC_Set_SCL_High(void)
{
    GPIO_SetBits(Sensor8_GPIO, Sensor8_IIC_SCL);
    
}
// End of void Sensor8_IIC_SCL_High(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_SCL_Low
* Description    :  SHT30����SCL
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sensor8_IIC_Set_SCL_Low(void)
{
    GPIO_ResetBits(Sensor8_GPIO, Sensor8_IIC_SCL);
    
}
// End of void Sensor8_IIC_SCL_Low(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_Set_SDA_Input
* Description    :  ����SDA�ܽ�Ϊ����ģʽ
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sensor8_IIC_Set_SDA_Input(void)
{
    Sensor8_GPIO->CRL &= 0xFF0FFFFF;									// A��ܽ� PIN5[23:20]
    Sensor8_GPIO->CRL |= 8 << 20;										// ��Ϊ��������ģʽ
}
// End of void Sensor8_IIC_Set_SDA_Input(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_Set_SDA_Output
* Description    :  ����SDA�ܽ�Ϊ���ģʽ
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sensor8_IIC_Set_SDA_Output(void)
{
     Sensor8_GPIO->CRL &= 0xFF0FFFFF;									// A��ܽ� PIN5[23:20]
   	 Sensor8_GPIO->CRL |= 3 << 20;										// ��Ϊ�������ģʽ 
}
// End of void Sensor8_IIC_Set_SDA_Output(void)

/*******************************************************************************
//*                           ����@2017-03-21
//* Function Name  :  Sensor8_IIC_Set_SCL_Output
//* Description    :  ����SCL�ܽ�Ϊ���ģʽ
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//void Sensor8_IIC_Set_SCL_Output(void)
//{
//    NRF_GPIO->DIRSET = (1UL << Sensor8_IIC_SCL_PIN);
//    
//}// End of void Sensor8_IIC_Set_SCL_Output(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_SDA_Read
* Description    :  ��ȡSDA�ܽŵ�ƽ
* Input          :  None
* Output         :  None
* Return         :  �ߵ�ƽ����1 �͵�ƽ����0
*******************************************************************************/
u8 Sensor8_IIC_SDA_Read(void)
{
    return(GPIO_ReadInputDataBit(Sensor8_GPIO, Sensor8_IIC_SDA));
    
}
// End of u8 Sensor8_IIC_SDA_Read(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_SCL_Read
* Description    :  ��ȡSCL�ܽŵ�ƽ
* Input          :  None
* Output         :  None
* Return         :  �ߵ�ƽ����1 �͵�ƽ����0
*******************************************************************************/
u8 Sensor8_IIC_SCL_Read(void)
{
    return(GPIO_ReadInputDataBit(Sensor8_GPIO, Sensor8_IIC_SCL));
    
}
// End of u8 Sensor8_IIC_SCL_Read(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_Port_Init
* Description    :  SHT30�˿ڳ�ʼ��
* Input          :  None
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 Sensor8_IIC_Init(void)
{
    u8 Transfer_Succeeded; 

	Sensor8_IIC_Pin_Init();													// ��ʼ���ܽ�
	
	// ���ýṹ��
//    Sensor8_IIC_s.Dealy                 = Sensor8_IIC_Delay;
    Sensor8_IIC_s.Delay_us              = Sensor8_IIC_Delay_us;
//    Sensor8_IIC_s.Pin_Init              = Sensor8_IIC_Pin_Init;
    Sensor8_IIC_s.SDA_High              = Sensor8_IIC_Set_SDA_High;
    Sensor8_IIC_s.SDA_Low               = Sensor8_IIC_Set_SDA_Low;
    Sensor8_IIC_s.SCL_High              = Sensor8_IIC_Set_SCL_High;
    Sensor8_IIC_s.SCL_Low               = Sensor8_IIC_Set_SCL_Low;
    Sensor8_IIC_s.Set_SDA_Input         = Sensor8_IIC_Set_SDA_Input; 
    Sensor8_IIC_s.Set_SDA_Output        = Sensor8_IIC_Set_SDA_Output; 
//    Sensor8_IIC_s.Set_SCL_Output        = Sensor8_IIC_Set_SCL_Output;   
    Sensor8_IIC_s.SDA_Read              = Sensor8_IIC_SDA_Read; 
    Sensor8_IIC_s.SCL_Read              = Sensor8_IIC_SCL_Read;
    Sensor8_IIC_s.Wait_Scl_Free_Timeout = SENSOR8_TIMEOUT;
    
    Transfer_Succeeded = SW_IIC_Init(&Sensor8_IIC_s);							// ��ʼ��IICͨ��
    
    return (Transfer_Succeeded);
       
}
// End of u8  Sensor8_IIC_Port_Init(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_Variable_Init
* Description    :  ��ʪ�ȱ�����ʼ��
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sensor8_IIC_Variable_Init(void)
{
  
}// End of void Sensor8_IIC_Variable_Init(void)


/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_Read_Register
* Description    :  ���Ĵ���
* Input          :  u8 Chip_Addr	оƬ��ַ
*					u16 usRead_Addr Ҫ��ȡ�ĵ�ַ
*                   u8* pBuffer     ����ָ��
*                   u16 usRead_Len  ��ȡ����
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 Sensor8_IIC_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u16 usRead_Addr, u8* pBuffer, u16 usRead_Len)
{
    u8 Transfer_Succeeded = 1;
    u8 ucRead_Addr[2];
    u8 Write_Addr;
    u8 Read_Addr;

	Write_Addr = Chip_Addr << 1;
	Read_Addr  = Write_Addr | 0x01;
    
    if(usRead_Len == 0)
    {
        return 1;
    }
    
    ucRead_Addr[0] = (usRead_Addr >> 8) & 0xFF;
    ucRead_Addr[1] = (usRead_Addr >> 0) & 0xFF;
    Transfer_Succeeded &= SW_IIC_Transfer(IIC_s, Write_Addr, ucRead_Addr, 2, SW_IIC_DONT_SEND_STOP);
    Transfer_Succeeded &= SW_IIC_Transfer(IIC_s, Read_Addr, pBuffer, usRead_Len, SW_IIC_NEED_SEND_STOP);

	//printf("pBuffer = %d", *pBuffer);
    STM32_Delay_us(100);

    return Transfer_Succeeded;
    
}
// End of u8 Sensor8_IIC_Read_Register(u16 usRead_Addr, u8* pBuffer, u16 usRead_Len)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  Sensor8_IIC_Write_Register
* Description    :  доƬ�Ĵ���
* Input          :  u16 usWrite_Addr    Ҫд��ĵ�ַ
*                   u8* pBuffer         ����ָ��
*                   u16 usWrite_Len     д�볤�� 
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 Sensor8_IIC_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len)
{
    unsigned char Transfer_Succeeded = 1;
    u8 ucWrite_Addr[2];

    ucWrite_Addr[0] = (usWrite_Addr >> 8) & 0xFF;
    ucWrite_Addr[1] = (usWrite_Addr >> 0) & 0xFF;
    
    // ����д����
    Transfer_Succeeded &= SW_IIC_Transfer(&Sensor8_IIC_s, SHT30_WRITE_ADDR, ucWrite_Addr, 2, SW_IIC_DONT_SEND_STOP);
    
    // ���ȱ���
    if (usWrite_Len == 0)
    {
        return 0;
    }

    // ��������
    while (usWrite_Len-- && Transfer_Succeeded)
    {
        Transfer_Succeeded &= SW_IIC_Write_Byte(&Sensor8_IIC_s, *pBuffer);
        pBuffer++;
    }


    // ����ֹͣλ
    Transfer_Succeeded &= SW_IIC_Stop(&Sensor8_IIC_s);

    STM32_Delay_us(100);
    
    return Transfer_Succeeded;
    
}
// End of u8 Temp_Humi_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len)

/******************* (C) COPYRIGHT 2017 ���� **************END OF FILE*********/

