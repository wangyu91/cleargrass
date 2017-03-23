/******************** (C) COPYRIGHT 2016 王宇 **********************************
* File Name          :  SW_IIC.c
* Author             :  王宇
* CPU Type           :  STM32f43
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  22/03/2017
* Description        :  I2C软实现
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "SW_IIC.h"

/* Private Variables ---------------------------------------------------------*/

/* Private Typedefs ----------------------------------------------------------*/

/* Private Function Prototypes -----------------------------------------------*/
unsigned char SW_IIC_Init(SW_IIC_t * SW_IIC_s);							// IIC 初始化
unsigned char SW_IIC_Start(SW_IIC_t * SW_IIC_s);						// IIC 启动信号
unsigned char SW_IIC_Stop(SW_IIC_t * SW_IIC_s);							// IIC 停止信号
unsigned char SW_IIC_Write_Byte(SW_IIC_t * SW_IIC_s, unsigned char IIC_Data);	// 写一个字节
unsigned char SW_IIC_Read_Byte(SW_IIC_t * SW_IIC_s, unsigned char * Byte_Buff, unsigned char Need_ACK);	// 读一个字节
unsigned char SW_IIC_Transfer(SW_IIC_t * SW_IIC_s, 
							  unsigned char   Chip_Addr, 
							  unsigned char*  Data, 
							  unsigned short  Data_Len, 
							  unsigned char   Stop_Condition);			// 发送数据 通信交互
static void   		 IIC_Delay(unsigned char count);					// 延时
static unsigned char SW_IIC_Wait_SCL_Turn_High(SW_IIC_t * SW_IIC_s);	// 等待SCL被从机释放
//void SW_IIC_ClearBus(void);

/* Private Functions ---------------------------------------------------------*/
/*******************************************************************************
*                           王宇@2017-03-22
* Function Name  :  SW_IIC_Init
* Description    :  I2C端口初始化 
* Input          :  SW_IIC_t *SW_IIC_s  I2C参数
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
unsigned char SW_IIC_Init(SW_IIC_t * SW_IIC_s)
{
	unsigned char Bus_Clear;											// 总线清空标志
	unsigned char i;

	// 释放总线
	SW_IIC_s->SDA_High();												// 空闲时间SDA为高电平且
	SW_IIC_s->SCL_High();												// SCL也为高电平
	STM32_Delay_us(4);

	if (SW_IIC_s->SDA_Read() == 1 && SW_IIC_s->SCL_Read() == 1)
	{
		Bus_Clear = 1;													// 总线空闲可用
	}
	else if (SW_IIC_s->SCL_Read() == 1)									// SDA不是高电平
	{
		Bus_Clear = 0;													// 尚有数据待发送

		for (i = 0; i < 18; i++)										// 继续发送2Byte特数据和2位校验
		{
			SW_IIC_s->SCL_Low();										//*!-> ↑不确定 <-!*/
			STM32_Delay_us(4);
			SW_IIC_s->SCL_High();
			STM32_Delay_us(4);

			if (SW_IIC_s->SDA_Read() == 1)								// 数据发送完成
			{
				Bus_Clear = 1;											// 总线就绪
				break;
			}
		}
	}
	else
	{
		Bus_Clear = 0;
	}

	return Bus_Clear;
	
}
// end of unsigned char SW_IIC_Init(SW_IIC_t * SW_IIC_s)

/*******************************************************************************
*                           王宇@2017-03-22
* Function Name  :  SW_IIC_Wait_SCL_Turn_High
* Description    :  等待从机释放SCL 
* Input          :  SW_IIC_t *SW_IIC_s  I2C参数
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
static unsigned char SW_IIC_Wait_SCL_Turn_High(SW_IIC_t * SW_IIC_s)
{
	unsigned int volatile Timeout_Counter = SW_IIC_s->Wait_Scl_Free_Timeout;

    // 释放SCL
    SW_IIC_s->SCL_High();

    // 等待时钟为高
    while (SW_IIC_s->SCL_Read() == 0)
    {
        if (Timeout_Counter == 0)
        {
            // 超时
            return 0;
        }
        else
        {
            Timeout_Counter--;
            STM32_Delay_us(1);
        }
    }
    
    STM32_Delay_us(4);
    
    return 1;
    
}
// end of static unsigned char SW_IIC_Wait_SCL_Turn_High(SW_IIC_t * SW_IIC_s)

/*******************************************************************************
*                           王宇@2017-03-22
* Function Name  :  SW_IIC_Start
* Description    :  产生I2C起始信号
* Input          :  SW_IIC_t * SW_IIC_s  操作指针
* Output         :  None
* Return         :  成功返回1 失败返回0
*******************************************************************************/
unsigned char SW_IIC_Start(SW_IIC_t * SW_IIC_s)
{
	SW_IIC_s->SDA_High();												// 拉高SDA
	STM32_Delay_us(4);
	
	if (SW_IIC_Wait_SCL_Turn_High(SW_IIC_s) == 0)						// 等待SCL拉高
	{
		return 0;
	}

	SW_IIC_s->SDA_Low();												// 拉低SDA产生起始信息
	STM32_Delay_us(4);
	SW_IIC_s->SCL_Low();												// SCL拉低响应
	STM32_Delay_us(4);

	return 1;
}
// end of unsigned char SW_IIC_Start(SW_IIC_t * SW_IIC_s)

/*******************************************************************************
*                           王宇@2017-03-22
* Function Name  :  SW_I2C_Stop
* Description    :  产生I2C停止信号
* Input          :  SW_IIC_t *SW_IIC_s  操作指针
* Output         :  None
* Return         :  成功返回1 失败返回0
*******************************************************************************/
unsigned char SW_IIC_Stop(SW_IIC_t * SW_IIC_s)
{
	SW_IIC_s->SDA_Low();												// SDA为低
	STM32_Delay_us(4);

	if (SW_IIC_Wait_SCL_Turn_High(SW_IIC_s) == 0)						// 等待SCL拉高
	{
		return 0;
	}

	SW_IIC_s->SDA_High();												// 拉高SDA以产生停止信号
	STM32_Delay_us(4);
	
	return 1;
}
// end of unsigned char SW_IIC_Stop(SW_IIC_t * SW_IIC_s)

/*******************************************************************************
*                           王宇@2017-03-22
* Function Name  :  SW_IIC_Write_Byte
* Description    :  IIC写数据
* Input          :  SW_IIC_t      *SW_IIC_s   操作结构体指针
*                   unsigned char IIC_Data    要发送的数据       
* Output         :  None
* Return         :  成功返回1 失败返回0
*******************************************************************************/
unsigned char SW_IIC_Write_Byte(SW_IIC_t * SW_IIC_s, unsigned char IIC_Data)
{
	unsigned char Succeed = 1;
	unsigned char i;

	SW_IIC_s->SCL_Low();												// 拉低SCL
	STM32_Delay_us(4);

	for (i = 0x80; i > 0; i >>= 1)										// 发送数据 从高位开始
	{
		if (IIC_Data & i)
		{
			SW_IIC_s->SDA_High();
		}
		else
		{
			SW_IIC_s->SDA_Low();
		}

		if (SW_IIC_Wait_SCL_Turn_High(SW_IIC_s) == 0)					// 等待SCL空闲
		{
			Succeed = 0;												// 超时返回
			break;
		}

		SW_IIC_s->SCL_Low();											// 拉低SCL等待下一次发送
		STM32_Delay_us(4);
	}

	Succeed &= SW_IIC_Wait_SCL_Turn_High(SW_IIC_s);						// 等待SCL空闲接收ACK

	if (SW_IIC_s->SDA_Read() & SW_IIC_No_ACK)							// 读取ACK应答
	{
		Succeed = 0x00;													// 没有收到应答
	}
	else
	{
		Succeed &= 0x01;												// 收到应带
	}

	SW_IIC_s->SCL_Low();												// 拉低SCL准备下一次发送
	STM32_Delay_us(4);

	return Succeed;
	
}
// end of unsigned char SW_IIC_Write_Byte(SW_IIC_t * SW_IIC_s, unsigned char IIC_Data)

/*******************************************************************************
*                           王宇@2017-03-22
* Function Name  :  SW_IIC_Read_Byte
* Description    :  IIC读取一个字节数据
* Input          :  SW_IIC_t      *SW_IIC_s      操作结构体指针
*                   unsigned char *Byte_Buff     读取数据缓存
*                   unsigned char  Need_Ack      置一是发送ACK
* Output         :  None
* Return         :  成功返回1 失败返回0
*******************************************************************************/
unsigned char SW_IIC_Read_Byte(SW_IIC_t * SW_IIC_s, unsigned char * Byte_Buff, unsigned char Need_ACK)
{
	unsigned char Byte_Read = 0;
	unsigned char Succeed = 1;
	unsigned char i;

	for (i = 0x80; i > 0; i >>= 1)										// 接收数据 从高位开始
	{
		if (SW_IIC_Wait_SCL_Turn_High(SW_IIC_s) == 0)					// 等待SCL空闲
		{
			Succeed = 0;
			break;
		}

		if (SW_IIC_s->SDA_Read())										// 根据SDA电平高低
		{
			Byte_Read |= i;												// 读取数据
		}

		SW_IIC_s->SCL_Low();											// 下拉SCL为下一次接收做准备
		STM32_Delay_us(4);
		
	}

	*Byte_Buff = Byte_Read;												// 接收数据存入BUFFER

	if (Need_ACK == SW_IIC_SEND_ACK)									// 发送ACK
	{
		SW_IIC_s->SDA_Low();
	}
	else
	{
		SW_IIC_s->SDA_High();
	}
	STM32_Delay_us(4);

	if (SW_IIC_Wait_SCL_Turn_High(SW_IIC_s) == 0)						// 等待从机响应
	{
		Succeed = 0;
	}

	SW_IIC_s->SCL_Low();												// 拉低SCL准备下一次接收
	STM32_Delay_us(4);

	return Succeed;
	
}
// end of unsigned char SW_IIC_Read_Byte(SW_IIC_t * SW_IIC_s, unsigned char * Byte_Buff, unsigned char Need_ACK)

/*******************************************************************************
*                           王宇@2017-03-22
* Function Name  :  SW_IIC_Transfer
* Description    :  I2C发送数据
* Input          :  SW_I2C_Typedef *SW_I2C  操作指针
*                   unsigned char Chip_Addr 芯片地址
*                   unsigned char  *Data    数据指针
*                   unsigned short Data_Len 数据长度
*                   unsigned char Stop_Condition    置一时发送停止信号
* Output         :  None
* Return         :  成功返回1 失败返回0
*******************************************************************************/
unsigned char SW_IIC_Transfer(SW_IIC_t * SW_IIC_s, unsigned char Chip_Addr, unsigned char *Data, unsigned short Data_Len, unsigned char Stop_Condition)
{
	unsigned char Succeed = 1;

    Succeed &= SW_IIC_Start(SW_IIC_s);									// 通信开始
    Succeed &= SW_IIC_Write_Byte(SW_IIC_s, Chip_Addr);					// 发送地址

    // 长度保护
    if (Data_Len == 0)
    {
        return 0;
    }
    
    // 判定传输方向
    if (Chip_Addr & 0x01)												// 接收
    {
        while (Data_Len-- && Succeed)									// 接收数据
        {
            // 最后一个字节发送NO ACK
            if (Data_Len == 0)
            {
                Succeed &= SW_IIC_Read_Byte(SW_IIC_s, Data, SW_IIC_DONT_SEND_ACK);
            }
            else
            {
                Succeed &= SW_IIC_Read_Byte(SW_IIC_s, Data, SW_IIC_SEND_ACK);
            }
            Data++;
        }
    }
    else																// 发送
    {
        while (Data_Len-- && Succeed)									// 发送数据
        {
            Succeed &= SW_IIC_Write_Byte(SW_IIC_s, *Data);
            Data++;
        }
    }

    // 有停止信号或者发送失败
    if (Stop_Condition || (Succeed == 0))
    {
        Succeed &= SW_IIC_Stop(SW_IIC_s);
    }

    return Succeed;
    
}
// end of unsigned char SW_IIC_Transfer(SW_IIC_t * SW_IIC_s, unsigned char Chip_Addr, unsigned char *Data, unsigned short Data_Len, unsigned char Stop_Condition)

/*******************************************************************************
*                           王宇@2017-03-22
* Function Name  :  IIC_Delay
* Description    :  IIC 时延
* Input          :  unsigned char count     时延计数
* Output         :  None
* Return         :  None
*******************************************************************************/
static void IIC_Delay(unsigned char count)
{
	unsigned char i, j;

	for (i = 0; i < count; i++)
	{
		for (j = 0; j < IIC_DELAY_COUNT; j++);
	}
}
// end of static void IIC_Delay(unsigned char count)
/******************* (C) COPYRIGHT 2017 王宇 ********END OF FILE***************/
