/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  Sgpc10_Driver.c
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  31/03/2017
* Description        :  Sgpc10 TVOCӦ�ó���
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Sgpc10_Driver.h"
#include "SW_IIC.h"
#include "Sensirion_common.h"
#include "Sgpc1x_featureset.h"

/* Private variables ---------------------------------------------------------*/
        
/* Private function prototypes -----------------------------------------------*/
u8 	Sgpc10_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u16 usRead_Addr, u8* pBuffer, u16 usRead_Len);	
//float	BH1721_CalcData(u16 usValue);									// ����ԭʼ����
u8 Sgpc10_Init_Air_Quality(SW_IIC_t* IIC_s);							// ������������ʼ��

u8 	Sgpc10_Get_Data(SW_IIC_t *IIC_s, u16 *Tvoc_Data, u16 *Co2_Data);	// ��ȡ��������
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                           ����@2017-03-31
* Function Name  :  Sgpc10_Read_Register
* Description    :  ���Ĵ���
* Input          :  SW_IIC_t*	IIC_s		IIC��·
					u8			Chip_Addr	оƬ��ַ
					u8			usRead_Addr ָ��
					u8*			pBuffer		���ݻ���
					u16			usRead_Len  ���ݳ���
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 Sgpc10_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u16 usRead_Addr, u8* pBuffer, u16 usRead_Len)
{
	u8 Succeed = 1;
	u8 Write_Addr;
	u8 Read_Addr;
	u8 ucRead_Addr[2];

	Write_Addr = Chip_Addr << 1;
	Read_Addr  = Write_Addr | 0x01;

    if (usRead_Len == 0)
   	{
		return 1;
    }
	ucRead_Addr[0] = (usRead_Addr >> 8) & 0xFF;
    ucRead_Addr[1] = (usRead_Addr >> 0) & 0xFF;
    
    Succeed &= SW_IIC_Transfer(IIC_s, Write_Addr, ucRead_Addr, 2, SW_IIC_DONT_SEND_STOP);	// ��Ҫ����ֹͣ�ź�
	// ��ȡ������Ҫ����23ms
    STM32_Delay_ms(30);
    Succeed &= SW_IIC_Transfer(IIC_s, Read_Addr, pBuffer, usRead_Len, SW_IIC_NEED_SEND_STOP);

    STM32_Delay_us(100);

    return Succeed;														// �ɹ�����1
}
// end of u8 Sgpc10_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u8 usRead_Addr, u8* pBuffer, u16 usRead_Len)

/*******************************************************************************
//*                           ����@2017-03-28
//* Function Name  :  BH1721_CalcData
//* Description    :  ��ǿ����
//* Input          :  u16 usValue 16bit��ǿ
//* Output         :  None
//* Return         :  float�͹�ǿ
//*******************************************************************************/
//float BH1721_CalcData(u16 usValue)
//{
//    // calculate Light Intensity [lx]
//    // Result = rawValue / 1.2 / (2^16-1)

//    float fLight = 0.0;
//    fLight  = (usValue / 1.2) / 65535.0;
//    
//    return fLight ;
//    
//}
// End of float SHT30_CalcTemp(u16 usValue)

/*******************************************************************************
*                           ����@2017-03-31
* Function Name  :  Sgpc10_Init_Air_Quality
* Description    :  ������������ʼ��
* Input          :  SW_IIC_t*	IIC_s	IIC��·ѡ��
* Output         :  None
* Return         :  �ɹ�1 ʧ��0
*******************************************************************************/
u8 Sgpc10_Init_Air_Quality(SW_IIC_t* IIC_s)
{
	u8 Succeed = 1;
	u8 ucWrite_Addr[2];

    ucWrite_Addr[0]  =  (CMD_INIT_AIR_QUALITY >> 8) & 0xFF;
    ucWrite_Addr[1]  =  (CMD_INIT_AIR_QUALITY >> 0) & 0xFF;
	Succeed		    &=  SW_IIC_Transfer(IIC_s, SGPC10_WRITE_ADDR, ucWrite_Addr, 2, SW_IIC_NEED_SEND_STOP);

	return Succeed;
}

/*******************************************************************************
*                           ����@2017-03-28
* Function Name  :  Sgpc10_Get_Data
* Description    :  ��ȡ������������
* Input          :  SW_IIC_t	*IIC_s		IIC��·
					u16 		*Tvoc_Data  TVOC����
					u16			*Co2_Data	Co2����
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 Sgpc10_Get_Data(SW_IIC_t *IIC_s, u16 *Tvoc_Data, u16 *Co2_Data)
{
	u8 ucResult = 0;
	u8 ucRead_Data[6];
	u8 Data[4];															// �����ݴ� Co2(0,1) Tvoc(2,3)
	u8 i, y;

	ucResult = Sgpc10_Read_Register(IIC_s, 0x58, CMD_MEASURE_AIR_QUALITY, ucRead_Data, 6);

	if (ucResult)
	{
		// ͨ����CRC
		for (i = 0, y = 0; i < 6; i += SGP_WORD_LEN + CRC8_LEN, y += SGP_WORD_LEN) 
		{

        	if (sensirion_common_check_crc(&ucRead_Data[i], SGP_WORD_LEN, ucRead_Data[i + SGP_WORD_LEN]) != 0) 
        	{
            	ucResult = 0;
        	}
        	*((u16 *)&Data[y]) = *((u16 *)&ucRead_Data[i]);
		}
	}

	*Tvoc_Data = (Data[2] << 8) + Data[3];
	*Co2_Data  = (Data[0] << 8) + Data[1];

	return(ucResult);
	
}
// end of u8 Sgpc10_Get_Data(SW_IIC_t *IIC_s, float *fLight)

/******************* (C) COPYRIGHT 2017 ���� **************END OF FILE*********/


