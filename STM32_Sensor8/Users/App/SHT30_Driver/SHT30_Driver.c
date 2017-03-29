/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  SHT30_Driver.c
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  23/03/2017
* Description        :  SHT30温湿度应用程序
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "SHT30_Driver.h"
//#include "nrf_gpio.h"

/* Private variables ---------------------------------------------------------*/
        
/* Private function prototypes -----------------------------------------------*/
u8    SHT30_Read_SN(u8 *ucSN);                                          // 读取SN
u8    SHT30_Soft_Reset(void);                                           // 软复位
u8    SHT30_Check_CRC(u8 *ucData, u8 ucLen, u8 ucCheck_Sum);            // CRC校验
u8 	  SHT30_Get_Data(float *fTemp, float *fHumi);                 		// 获取温湿度
float SHT30_CalcTemp(u16 usValue);                              		// 计算温度
float SHT30_CalcHumi(u16 usValue);										// 计算湿度

u8 	  SHT30_IIC_Get_Data(SW_IIC_t *IIC_s, float *fTemp, float *fHumi);

u8	  SHT30_IIC_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u16 usRead_Addr, u8* pBuffer, u16 usRead_Len);// 读寄存器
u8    SHT30_IIC_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len);// 写寄存器
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
*                           王宇@2017-03-23
* Function Name  :  SHT30_Read_SN
* Description    :  读SN
* Input          :  u8 读取的SN
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 SHT30_Read_SN(u8 *ucSN)
{

    return(SHT30_Read_Register(CMD_READ_SERIALNBR, ucSN, 4));

}
// End of u8 SHT30_Read_SN(u8 *ucSN)

/*******************************************************************************
*                           王宇@2017-03-23
* Function Name  :  SHT30_Get_Data
* Description    :  获取温湿度
* Input          :  float *fTemp    温度数据
*                   float *fHumi    湿度数据
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 SHT30_Get_Data(float *fTemp, float *fHumi)
{
    u8 ucResult = 0;
    u8 ucRead_Data[6];
    
    ucResult = SHT30_Read_Register(CMD_MEAS_CLOCKSTR_L, ucRead_Data, 6);
    
	u8 i;
	
    if (ucResult)
    {
        // 通过了CRC
        if (SHT30_Check_CRC(ucRead_Data, 2, ucRead_Data[2]) &&  SHT30_Check_CRC(&ucRead_Data[3], 2, ucRead_Data[5]))
        {
            // 计算温湿度
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
*                           王宇@2017-03-23
* Function Name  :  SHT30_CalcTemp
* Description    :  温度计算
* Input          :  u16 usValue 16bit温度
* Output         :  None
* Return         :  float型温度
*******************************************************************************/
float SHT30_CalcTemp(u16 usValue)
{
    // calculate temperature [°C]
    // T = -45 + 175 * rawValue / (2^16-1)

    float fTemp = 0.0;
    fTemp  = (175.0 * usValue) / 65535.0;
    fTemp -= 45.0;
    
    return fTemp ;
    
}
// End of float SHT30_CalcTemp(u16 usValue)

/*******************************************************************************
*                           王宇@2017-03-23
* Function Name  :  SHT30_CalcHumi
* Description    :  湿度计算
* Input          :  u16 usValue 16bit湿度
* Output         :  None
* Return         :  float型湿度
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
*                           王宇@2017-03-23
* Function Name  :  SHT30_Check_CRC
* Description    :  CRC判断
*                   复位后需要至少delay 1ms
* Input          :  None
* Output         :  None
* Return         :  1成功 0失败
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
*                           王宇@2016-12-28
* Function Name  :  SHT30_Soft_Reset
* Description    :  软复位 
*                   复位后需要至少delay 1ms
* Input          :  None
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 SHT30_Soft_Reset(void)
{
    unsigned char Transfer_Succeeded = 1;
    u8 ucWrite_Addr[2];

    ucWrite_Addr[0] = (CMD_SOFT_RESET >> 8) & 0xFF;
    ucWrite_Addr[1] = (CMD_SOFT_RESET >> 0) & 0xFF;
    
    // 发送写命令
    Transfer_Succeeded = SW_IIC_Transfer(&SHT30_s, SHT30_WRITE_ADDR, ucWrite_Addr, 2, SW_IIC_NEED_SEND_STOP);

    STM32_Delay_ms(5);
    
    return(Transfer_Succeeded);

}
// End of u8 SHT30_Soft_Reset(void)

/*******************************************************************************
*                           王宇@2017-03-23
* Function Name  :  SHT30_IIC_Get_Data
* Description    :  获取温湿度
* Input          :  float *fTemp    温度数据
*                   float *fHumi    湿度数据
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 SHT30_IIC_Get_Data(SW_IIC_t *IIC_s, float *fTemp, float *fHumi)
{
    u8 ucResult = 0;
    u8 ucRead_Data[6];
    
    ucResult = SHT30_IIC_Read_Register(IIC_s, 0x44, CMD_MEAS_CLOCKSTR_L, ucRead_Data, 6);
    
	u8 i;
	
    if (ucResult)
    {
        // 通过了CRC
        if (SHT30_Check_CRC(ucRead_Data, 2, ucRead_Data[2]) &&  SHT30_Check_CRC(&ucRead_Data[3], 2, ucRead_Data[5]))
        {
            // 计算温湿度
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
// End of u8 SHT30_IIC_Get_Data(float *fTemp, float *fHumi)


/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  SHT30_IIC_Read_Register
* Description    :  读寄存器
* Input          :  SW_IIC_t*	IIC_s	选IIC线
*					u8 Chip_Addr	芯片地址
*					u16 usRead_Addr 要读取的地址
*                   u8* pBuffer     缓存指针
*                   u16 usRead_Len  读取长度
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 SHT30_IIC_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u16 usRead_Addr, u8* pBuffer, u16 usRead_Len)
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
// End of u8 SHT30_IIC_Read_Register(u16 usRead_Addr, u8* pBuffer, u16 usRead_Len)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  SHT30_IIC_Write_Register
* Description    :  写芯片寄存器
* Input          :  u16 usWrite_Addr    要写入的地址
*                   u8* pBuffer         缓存指针
*                   u16 usWrite_Len     写入长度 
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 SHT30_IIC_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len)
{
    unsigned char Transfer_Succeeded = 1;
    u8 ucWrite_Addr[2];

    ucWrite_Addr[0] = (usWrite_Addr >> 8) & 0xFF;
    ucWrite_Addr[1] = (usWrite_Addr >> 0) & 0xFF;
    
    // 发送写命令
    Transfer_Succeeded &= SW_IIC_Transfer(&Sensor8_IIC_s, SHT30_WRITE_ADDR, ucWrite_Addr, 2, SW_IIC_DONT_SEND_STOP);
    
    // 长度保护
    if (usWrite_Len == 0)
    {
        return 0;
    }

    // 发送数据
    while (usWrite_Len-- && Transfer_Succeeded)
    {
        Transfer_Succeeded &= SW_IIC_Write_Byte(&Sensor8_IIC_s, *pBuffer);
        pBuffer++;
    }


    // 发送停止位
    Transfer_Succeeded &= SW_IIC_Stop(&Sensor8_IIC_s);

    STM32_Delay_us(100);
    
    return Transfer_Succeeded;
    
}
// End of u8 SHT30_IIC_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len)

/******************* (C) COPYRIGHT 2017 王宇 **************END OF FILE*********/
