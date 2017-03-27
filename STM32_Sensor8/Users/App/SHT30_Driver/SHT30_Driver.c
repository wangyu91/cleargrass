/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  SHT30_Driver.c
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  23/03/2017
* Description        :  SHT30��ʪ��Ӧ�ó���
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "SHT30_Driver.h"
//#include "nrf_gpio.h"

/* Private variables ---------------------------------------------------------*/
        
/* Private function prototypes -----------------------------------------------*/
u8    SHT30_Read_SN(u8 *ucSN);                                          // ��ȡSN
u8    SHT30_Soft_Reset(void);                                           // ��λ
u8    SHT30_Check_CRC(u8 *ucData, u8 ucLen, u8 ucCheck_Sum);            // CRCУ��
u8 	  SHT30_Get_Data(float *fTemp, float *fHumi);                 		// ��ȡ��ʪ��
float SHT30_CalcTemp(u16 usValue);                              		// �����¶�
float SHT30_CalcHumi(u16 usValue);										// ����ʪ��

u8 	  Sensor8_IIC_Get_Data(SW_IIC_t *IIC_s, float *fTemp, float *fHumi);
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                           ����@2017-03-23
* Function Name  :  SHT30_Read_SN
* Description    :  ��SN
* Input          :  u8 ��ȡ��SN
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 SHT30_Read_SN(u8 *ucSN)
{

    return(SHT30_Read_Register(CMD_READ_SERIALNBR, ucSN, 4));

}
// End of u8 SHT30_Read_SN(u8 *ucSN)

/*******************************************************************************
*                           ����@2017-03-23
* Function Name  :  SHT30_Get_Data
* Description    :  ��ȡ��ʪ��
* Input          :  float *fTemp    �¶�����
*                   float *fHumi    ʪ������
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 SHT30_Get_Data(float *fTemp, float *fHumi)
{
    u8 ucResult = 0;
    u8 ucRead_Data[6];
    
    ucResult = SHT30_Read_Register(CMD_MEAS_CLOCKSTR_L, ucRead_Data, 6);
    
	u8 i;
	
    if (ucResult)
    {
        // ͨ����CRC
        if (SHT30_Check_CRC(ucRead_Data, 2, ucRead_Data[2]) &&  SHT30_Check_CRC(&ucRead_Data[3], 2, ucRead_Data[5]))
        {
            // ������ʪ��
            *fTemp = SHT30_CalcTemp((ucRead_Data[0] << 8) + ucRead_Data[1]);
            *fHumi = SHT30_CalcHumi((ucRead_Data[3] << 8) + ucRead_Data[4]);
        }
        else
        {
            ucResult = 0;
        }
    }
    
    return(ucResult);

}
// End of u8 SHT30_Get_Data(float *fTemp, float *fHumi)

/*******************************************************************************
*                           ����@2017-03-23
* Function Name  :  SHT30_CalcTemp
* Description    :  �¶ȼ���
* Input          :  u16 usValue 16bit�¶�
* Output         :  None
* Return         :  float���¶�
*******************************************************************************/
float SHT30_CalcTemp(u16 usValue)
{
    // calculate temperature [��C]
    // T = -45 + 175 * rawValue / (2^16-1)

    float fTemp = 0.0;
    fTemp  = (175.0 * usValue) / 65535.0;
    fTemp -= 45.0;
    
    return fTemp ;
    
}
// End of float SHT30_CalcTemp(u16 usValue)

/*******************************************************************************
*                           ����@2017-03-23
* Function Name  :  SHT30_CalcHumi
* Description    :  ʪ�ȼ���
* Input          :  u16 usValue 16bitʪ��
* Output         :  None
* Return         :  float��ʪ��
*******************************************************************************/
float SHT30_CalcHumi(u16 usValue)
{
    // calculate relative humidity [%RH]
    // RH = rawValue / (2^16-1) * 100
    
    float fHumi = 0.0;
    fHumi  = (100.0 * usValue) / 65535.0;
    
    return fHumi;
    
}
// End of float SHT30_CalcHumi(u16 usValue)
    
/*******************************************************************************
*                           ����@2017-03-23
* Function Name  :  SHT30_Check_CRC
* Description    :  CRC�ж�
*                   ��λ����Ҫ����delay 1ms
* Input          :  None
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 SHT30_Check_CRC(u8 *ucData, u8 ucLen, u8 ucCheck_Sum)
{
    u8 bit ;

    u8 crc = 0xFF ;

    u8 i ;

    for(i = 0; i < ucLen; i++)
    {
        crc ^= ucData[i];
        
        for(bit = 8; bit > 0; --bit)
        {
            if(crc & 0x80)
            {
                crc = (crc << 1 ) ^ 0x131 ;
            }
            else 
            {
                crc = crc << 1;
            }
        }
    }

    if(crc != ucCheck_Sum)
	{
		return 0;
	}
	else
	{
		return 1;
	}
    
}
// u8 SHT30_Check_CRC(u8 *ucData, u8 ucLen, u8 ucCheck_Sum)
    
/*******************************************************************************
*                           ����@2016-12-28
* Function Name  :  SHT30_Soft_Reset
* Description    :  ��λ 
*                   ��λ����Ҫ����delay 1ms
* Input          :  None
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 SHT30_Soft_Reset(void)
{
    unsigned char Transfer_Succeeded = 1;
    u8 ucWrite_Addr[2];

    ucWrite_Addr[0] = (CMD_SOFT_RESET >> 8) & 0xFF;
    ucWrite_Addr[1] = (CMD_SOFT_RESET >> 0) & 0xFF;
    
    // ����д����
    Transfer_Succeeded = SW_IIC_Transfer(&SHT30_s, SHT30_WRITE_ADDR, ucWrite_Addr, 2, SW_IIC_NEED_SEND_STOP);

    STM32_Delay_ms(5);
    
    return(Transfer_Succeeded);

}
// End of u8 SHT30_Soft_Reset(void)

/*******************************************************************************
*                           ����@2017-03-23
* Function Name  :  SHT30_Get_Data
* Description    :  ��ȡ��ʪ��
* Input          :  float *fTemp    �¶�����
*                   float *fHumi    ʪ������
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 Sensor8_IIC_Get_Data(SW_IIC_t *IIC_s, float *fTemp, float *fHumi)
{
    u8 ucResult = 0;
    u8 ucRead_Data[6];
    
    ucResult = Sensor8_IIC_Read_Register(IIC_s, 0x44, CMD_MEAS_CLOCKSTR_L, ucRead_Data, 6);
    
	u8 i;
	
    if (ucResult)
    {
        // ͨ����CRC
        if (SHT30_Check_CRC(ucRead_Data, 2, ucRead_Data[2]) &&  SHT30_Check_CRC(&ucRead_Data[3], 2, ucRead_Data[5]))
        {
            // ������ʪ��
            *fTemp = SHT30_CalcTemp((ucRead_Data[0] << 8) + ucRead_Data[1]);
            *fHumi = SHT30_CalcHumi((ucRead_Data[3] << 8) + ucRead_Data[4]);
        }
        else
        {
            ucResult = 0;
        }
    }
    
    return(ucResult);

}
// End of u8 SHT30_Get_Data(float *fTemp, float *fHumi)


/******************* (C) COPYRIGHT 2017 ���� **************END OF FILE*********/
