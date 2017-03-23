/******************** (C) COPYRIGHT 2016 ���� **********************************
* File Name          :  SHT30_Init.c
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  23/03/2017
* Description        :  SHT30��ʪ�ȴ�������ʼ������
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "SHT30_Init.h"
#include "GPIO_LED.h"
#include "SW_IIC.h"


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
//void SHT30_Delay(void);                                            	// ��ʱ
//void SHT30_Delay_us(void);                                          	// us��ʱ
void SHT30_Pin_Init(void);                                          	// �ܽų�ʼ��
void SHT30_Set_SDA_High(void);                                          // ����������
void SHT30_Set_SDA_Low(void);                                           // ����������
void SHT30_Set_SCL_High(void);                                          // ����ʱ��
void SHT30_Set_SCL_Low(void);                                           // ����ʱ��
//void SHT30_Set_SDA_Input(void);                                     	// ����SDAΪ����ģʽ
//void SHT30_Set_SDA_Output(void);                                    	// ����SDAΪ���ģʽ
//void SHT30_Set_SCL_Output(void);                                    	// ����SCLΪ���ģʽ
u8   SHT30_SDA_Read(void);                                          	// ��ȡSDA��ƽ
u8   SHT30_SCL_Read(void);                                          	// ��ȡSCL��ƽ

void SHT30_Variable_Init(void);                                     	// SHT30������ʼ��
u8   SHT30_Init(void);                                         			// SHT30�˿ڳ�ʼ��    
u8 SHT30_Read_Register(u16 usRead_Addr, u8* pBuffer, u16 usRead_Len);// ���Ĵ���
u8 SHT30_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len);// д�Ĵ���

/* Private functions ---------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
//*                           ����@2017-03-21
//* Function Name  :  SHT30_Delay
//* Description    :  SHT30��ʱ
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//void SHT30_Delay(void)
//{
//    nrf_delay_us(4);
//    
//}// End of void SHT30_Delay(void)

/*******************************************************************************
//*                           ����@2017-03-21
//* Function Name  :  SHT30_Delay_us
//* Description    :  SHT30 us��ʱ
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//void SHT30_Delay_us(void)
//{
//    nrf_delay_us(1);
//    
//}// End of void SHT30_Delay_us(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  SHT30_Pin_Init
* Description    :  �ܽų�ʼ��                   
* Input          :  P0.12   -->   SHT30_SCL_PIN
*                   P0.13   -->   SHT30_SDA_PIN
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
void SHT30_Pin_Init(void)
{
    
    // ���ùܽ�Ϊ��©ģʽ
    GPIO_Pin_Config(RCC_APB2Periph_GPIOA, GPIO_Pin_2, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, GPIOA);	// SCL A2��
    GPIO_Pin_Config(RCC_APB2Periph_GPIOA, GPIO_Pin_3, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, GPIOA);   // SDA A3��
    
}
// End of u8  SHT30_Pin_Init(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  SHT30_SDA_High
* Description    :  SHT30����SDA
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SHT30_Set_SDA_High(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_3);
    
}
// End of void SHT30_SDA_High(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  SHT30_SDA_Low
* Description    :  SHT30����SDA
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SHT30_Set_SDA_Low(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_3);
    
}
// End of void SHT30_SDA_Low(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  SHT30_SCL_High
* Description    :  SHT30����SCL
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SHT30_Set_SCL_High(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
    
}
// End of void SHT30_SCL_High(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  SHT30_SCL_Low
* Description    :  SHT30����SCL
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SHT30_Set_SCL_Low(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
    
}
// End of void SHT30_SCL_Low(void)

/*******************************************************************************
//*                           ����@2017-03-21
//* Function Name  :  SHT30_Set_SDA_Input
//* Description    :  ����SDA�ܽ�Ϊ����ģʽ
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//void SHT30_Set_SDA_Input(void)
//{
//    NRF_GPIO->DIRCLR = (1UL << SHT30_SDA_PIN);
//    
//}// End of void SHT30_Set_SDA_Input(void)

/*******************************************************************************
//*                           ����@2017-03-21
//* Function Name  :  SHT30_Set_SDA_Output
//* Description    :  ����SDA�ܽ�Ϊ���ģʽ
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//void SHT30_Set_SDA_Output(void)
//{
//    NRF_GPIO->DIRSET = (1UL << SHT30_SDA_PIN);
//    
//}// End of void SHT30_Set_SDA_Output(void)

/*******************************************************************************
//*                           ����@2017-03-21
//* Function Name  :  SHT30_Set_SCL_Output
//* Description    :  ����SCL�ܽ�Ϊ���ģʽ
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//void SHT30_Set_SCL_Output(void)
//{
//    NRF_GPIO->DIRSET = (1UL << SHT30_SCL_PIN);
//    
//}// End of void SHT30_Set_SCL_Output(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  SHT30_SDA_Read
* Description    :  ��ȡSDA�ܽŵ�ƽ
* Input          :  None
* Output         :  None
* Return         :  �ߵ�ƽ����1 �͵�ƽ����0
*******************************************************************************/
u8 SHT30_SDA_Read(void)
{
    return(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3));
    
}
// End of u8 SHT30_SDA_Read(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  SHT30_SCL_Read
* Description    :  ��ȡSCL�ܽŵ�ƽ
* Input          :  None
* Output         :  None
* Return         :  �ߵ�ƽ����1 �͵�ƽ����0
*******************************************************************************/
u8 SHT30_SCL_Read(void)
{
    return(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2));
    
}
// End of u8 SHT30_SCL_Read(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  SHT30_Port_Init
* Description    :  SHT30�˿ڳ�ʼ��
* Input          :  None
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 SHT30_Init(void)
{
    u8 Transfer_Succeeded; 

	SHT30_Pin_Init();													// ��ʼ���ܽ�
	
	// ���ýṹ��
//    SHT30_s.Dealy                 = SHT30_Delay;
//    SHT30_s.Dealy_us              = SHT30_Delay_us;
//    SHT30_s.Pin_Init              = SHT30_Pin_Init;
    SHT30_s.SDA_High              = SHT30_Set_SDA_High;
    SHT30_s.SDA_Low               = SHT30_Set_SDA_Low;
    SHT30_s.SCL_High              = SHT30_Set_SCL_High;
    SHT30_s.SCL_Low               = SHT30_Set_SCL_Low;
//    SHT30_s.Set_SDA_Input         = SHT30_Set_SDA_Input; 
//    SHT30_s.Set_SDA_Output        = SHT30_Set_SDA_Output; 
//    SHT30_s.Set_SCL_Output        = SHT30_Set_SCL_Output;   
    SHT30_s.SDA_Read              = SHT30_SDA_Read; 
    SHT30_s.SCL_Read              = SHT30_SCL_Read;
    SHT30_s.Wait_Scl_Free_Timeout = (5 * 1000);
    
    Transfer_Succeeded = SW_IIC_Init(&SHT30_s);							// ��ʼ��IICͨ��
    
    return (Transfer_Succeeded);
       
}
// End of u8  SHT30_Port_Init(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  SHT30_Variable_Init
* Description    :  ��ʪ�ȱ�����ʼ��
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SHT30_Variable_Init(void)
{

    
}// End of void SHT30_Variable_Init(void)


/*******************************************************************************
//*                           ����@2017-03-21
//* Function Name  :  Temp_Humi_Start
//* Description    :  ������������������
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//void Temp_Humi_Start(void)
//{
//    u8 ucError      = 0; 
//    u8 ucRetry_Time = 3;
//    u16 usLen;
//    u8 *ucData;
//    

//    do
//    {
//        // ��λģ��
//        ucError = 0;

//        // ��ȡ����
//        ucError |= SHT2x_GetSerialNumber(Temp_Humi.SerialNumber);

//        // ��ȡ�û��Ĵ���ֵ
//        ucError |= SHT2x_ReadUserRegister(&Temp_Humi.User_Register);  

//        // ����Ϊ10bitʪ�� 13bit�¶�ģʽ
//        Temp_Humi.User_Register = (Temp_Humi.User_Register & ~SHT2x_RES_MASK) | SHT2x_RES_8_12BIT;
//        ucError |= SHT2x_WriteUserRegister(&Temp_Humi.User_Register); 

//        if (ucRetry_Time)
//        {
//            ucRetry_Time--;
//        }
//        
//    }while((ucError != 0) && ucRetry_Time);

//    
//    // �ɹ���ʼ��������
//    if (ucError == 0)
//    {
//        // ��մ����־λ

//        NRF_LOG_INFO("Init SHT30 Succrss!\r\n");


//    }
//    else
//    {
//        // ��λ�����������־λ


//        NRF_LOG_ERROR("Init SHT30 Failed!\r\n");
//    
//    }
//    

//    
//}// End of void Temp_Humi_Start(void)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  SHT30_Read_Register
* Description    :  ���Ĵ���
* Input          :  u16 usRead_Addr Ҫ��ȡ�ĵ�ַ
*                   u8* pBuffer     ����ָ��
*                   u16 usRead_Len  ��ȡ����
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 SHT30_Read_Register(u16 usRead_Addr, u8* pBuffer, u16 usRead_Len)
{
    u8 Transfer_Succeeded = 1;
    u8 ucRead_Addr[2];
    
    if(usRead_Len == 0)
    {
        return 1;
    }
    
    ucRead_Addr[0] = (usRead_Addr >> 8) & 0xFF;
    ucRead_Addr[1] = (usRead_Addr >> 0) & 0xFF;
    Transfer_Succeeded &= SW_IIC_Transfer(&SHT30_s, SHT30_WRITE_ADDR, ucRead_Addr, 2, SW_IIC_DONT_SEND_STOP);
    Transfer_Succeeded &= SW_IIC_Transfer(&SHT30_s, SHT30_READ_ADDR, pBuffer, usRead_Len, SW_IIC_NEED_SEND_STOP);

    STM32_Delay_us(100);

    return Transfer_Succeeded;
    
}
// End of u8 SHT30_Read_Register(u16 usRead_Addr, u8* pBuffer, u16 usRead_Len)

/*******************************************************************************
*                           ����@2017-03-21
* Function Name  :  SHT30_Write_Register
* Description    :  доƬ�Ĵ���
* Input          :  u16 usWrite_Addr    Ҫд��ĵ�ַ
*                   u8* pBuffer         ����ָ��
*                   u16 usWrite_Len     д�볤�� 
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 SHT30_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len)
{
    unsigned char Transfer_Succeeded = 1;
    u8 ucWrite_Addr[2];

    ucWrite_Addr[0] = (usWrite_Addr >> 8) & 0xFF;
    ucWrite_Addr[1] = (usWrite_Addr >> 0) & 0xFF;
    
    // ����д����
    Transfer_Succeeded &= SW_IIC_Transfer(&SHT30_s, SHT30_WRITE_ADDR, ucWrite_Addr, 2, SW_IIC_DONT_SEND_STOP);
    
    // ���ȱ���
    if (usWrite_Len == 0)
    {
        return 0;
    }

    // ��������
    while (usWrite_Len-- && Transfer_Succeeded)
    {
        Transfer_Succeeded &= SW_IIC_Write_Byte(&SHT30_s, *pBuffer);
        pBuffer++;
    }


    // ����ֹͣλ
    Transfer_Succeeded &= SW_IIC_Stop(&SHT30_s);

    STM32_Delay_us(100);
    
    return Transfer_Succeeded;
    
}
// End of u8 Temp_Humi_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len)

/******************* (C) COPYRIGHT 2017 ���� **************END OF FILE*********/

