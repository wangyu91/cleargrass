/******************** (C) COPYRIGHT 2017 ���� **********************************
* File Name          :  BH1721_Driver.c
* Author             :  ����
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  28/03/2017
* Description        :  BH1721����Ӧ�ó���
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "BH1721_Driver.h"
#include "SW_IIC.h"

/* Private variables ---------------------------------------------------------*/
        
/* Private function prototypes -----------------------------------------------*/
u8 		BH1721_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u8 usRead_Addr, u8* pBuffer, u16 usRead_Len);	// ��BH1721�Ĵ���
float	BH1721_CalcData(u16 usValue);									// ����ԭʼ����
u8 		BH1721_Check_CRC(u8 *ucData, u8 ucLen, u8 ucCheck_Sum);			// CRCУ��
u8 		BH1721_Get_Data(SW_IIC_t *IIC_s, float *fLight);				// ��ȡ��ǿ��
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                           ����@2017-03-28
* Function Name  :  BH1721_Read_Register
* Description    :  ���Ĵ���
* Input          :  SW_IIC_t*	IIC_s		IIC��·
					u8			Chip_Addr	оƬ��ַ
					u8			usRead_Addr ָ��
					u8*			pBuffer		���ݻ���
					u16			usRead_Len  ���ݳ���
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 BH1721_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u8 usRead_Addr, u8* pBuffer, u16 usRead_Len)
{
	u8 Succeed = 1;
	u8 Write_Addr;
	u8 Read_Addr;

	Write_Addr = Chip_Addr << 1;
	Read_Addr  = Write_Addr | 0x01;

    if (usRead_Len == 0)
   	{
		return 1;
    }

    Succeed &= SW_IIC_Transfer(IIC_s, Write_Addr, &usRead_Addr, 1, SW_IIC_NEED_SEND_STOP);	// ��Ҫ����ֹͣ�ź�
    Succeed &= SW_IIC_Transfer(IIC_s, Read_Addr, pBuffer, usRead_Len, SW_IIC_NEED_SEND_STOP);

    STM32_Delay_us(100);

    return Succeed;														// �ɹ�����1
}
// end of u8 BH1721_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u8 usRead_Addr, u8* pBuffer, u16 usRead_Len)

/*******************************************************************************
*                           ����@2017-03-28
* Function Name  :  BH1721_CalcData
* Description    :  ��ǿ����
* Input          :  u16 usValue 16bit��ǿ
* Output         :  None
* Return         :  float�͹�ǿ
*******************************************************************************/
float BH1721_CalcData(u16 usValue)
{
    // calculate Light Intensity [lx]
    // Result = rawValue / 1.2 / (2^16-1)

    float fLight = 0.0;
    fLight  = (usValue / 1.2) / 65535.0;
    
    return fLight ;
    
}
// End of float SHT30_CalcTemp(u16 usValue)

/*******************************************************************************
//*                           ����@2017-03-28
//* Function Name  :  BH1721_Check_CRC
//* Description    :  CRC�ж�
//*                   ��λ����Ҫ����delay 1ms
//* Input          :  None
//* Output         :  None
//* Return         :  1�ɹ� 0ʧ��
//*******************************************************************************/
//u8 BH1721_Check_CRC(u8 *ucData, u8 ucLen, u8 ucCheck_Sum)
//{
//    u8 bit ;

//    u8 crc = 0xFF ;

//    u8 i ;

//    for(i = 0; i < ucLen; i++)
//    {
//        crc ^= ucData[i];
//        
//        for(bit = 8; bit > 0; --bit)
//        {
//            if(crc & 0x80)
//            {
//                crc = (crc << 1 ) ^ 0x131 ;
//            }
//            else 
//            {
//                crc = crc << 1;
//            }
//        }
//    }

//    if(crc != ucCheck_Sum)
//	{
//		return 0;
//	}
//	else
//	{
//		return 1;
//	}
//    
//}
// u8 BH1721_Check_CRC(u8 *ucData, u8 ucLen, u8 ucCheck_Sum)

/*******************************************************************************
*                           ����@2017-03-28
* Function Name  :  BH1721_Get_Data
* Description    :  ��ȡ����ǿ��
* Input          :  SW_IIC_t	*IIC_s		IIC��·
					float 		*fLight     ��ǿ����
* Output         :  None
* Return         :  1�ɹ� 0ʧ��
*******************************************************************************/
u8 BH1721_Get_Data(SW_IIC_t *IIC_s, float *fLight)
{
	u8 ucResult = 0;
	u8 ucRead_Data[2];

	ucResult = BH1721_Read_Register(IIC_s, 0x23, CMD_L_Resolution, ucRead_Data, 2);

	if (ucResult)
	{
		// ͨ����CRC
//		if (BH1721_Check_CRC(u8 * ucData,u8 ucLen,u8 ucCheck_Sum)
//		{
			*fLight = BH1721_CalcData((ucRead_Data[0] << 8) + ucRead_Data[1]);
//		}
//		else
//		{
//			ucResult = 0;
//		}
	}

	return(ucResult);
	
}
// end of u8 BH1721_Get_Data(SW_IIC_t *IIC_s, float *fLight)

/******************* (C) COPYRIGHT 2017 ���� **************END OF FILE*********/

