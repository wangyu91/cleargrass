/******************** (C) COPYRIGHT 2016 王宇 **********************************
* File Name          :  Sensor8_IIC_Init.c
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  27/03/2017
* Description        :  一拖八IIC初始化
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Sensor8_IIC_Init.h"
#include "SW_IIC.h"


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
//void Sensor8_IIC_Delay(void);                                            	// 延时
void Sensor8_IIC_Delay_us(void);                                          	// us延时
void Sensor8_IIC_Pin_Init(void);                                          	// 管脚初始化
void Sensor8_IIC_Set_SDA_High(void);                                          // 拉高数据线
void Sensor8_IIC_Set_SDA_Low(void);                                           // 拉低数据线
void Sensor8_IIC_Set_SCL_High(void);                                          // 拉高时钟
void Sensor8_IIC_Set_SCL_Low(void);                                           // 拉低时钟
void Sensor8_IIC_Set_SDA_Input(void);                                     	// 设置SDA为输入模式
void Sensor8_IIC_Set_SDA_Output(void);                                    	// 设置SDA为输出模式
//void Sensor8_IIC_Set_SCL_Output(void);                                    	// 设置SCL为输出模式
u8   Sensor8_IIC_SDA_Read(void);                                          	// 读取SDA电平
u8   Sensor8_IIC_SCL_Read(void);                                          	// 读取SCL电平

void Sensor8_IIC_Variable_Init(void);                                     	// SHT30变量初始化
u8   Sensor8_IIC_Init(void);                                         			// SHT30端口初始化    
u8 Sensor8_IIC_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u16 usRead_Addr, u8* pBuffer, u16 usRead_Len);// 读寄存器
u8 Sensor8_IIC_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len);// 写寄存器
/* Private functions ---------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
//*                           王宇@2017-03-21
//* Function Name  :  Sensor8_IIC_Delay
//* Description    :  SHT30延时
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
*                           王宇@2017-03-27
* Function Name  :  Sensor8_IIC_Delay_us
* Description    :  IIC us延时
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
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_Pin_Init
* Description    :  管脚初始化                   
* Input          :  P0.12   -->   Sensor8_IIC_SCL_PIN
*                   P0.13   -->   Sensor8_IIC_SDA_PIN
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
void Sensor8_IIC_Pin_Init(void)
{
    
    // 设置管脚为开漏模式
    GPIO_Pin_Config(Sensor8_IIC_GPIO_RCC, Sensor8_IIC_SDA|Sensor8_IIC_SCL, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, Sensor8_GPIO);   // SDA|A5脚 SCL|A4脚 开漏输出 速率50MHz 
   // GPIO_Pin_Config(Sensor8_IIC_GPIO_C, Sensor8_IIC_SCL, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, Sensor8_IIC_GPIO);	// SCL A6脚
}
// End of u8  Sensor8_IIC_Pin_Init(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_SDA_High
* Description    :  SHT30拉高SDA
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
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_SDA_Low
* Description    :  SHT30拉低SDA
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
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_SCL_High
* Description    :  SHT30拉高SCL
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
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_SCL_Low
* Description    :  SHT30拉低SCL
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
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_Set_SDA_Input
* Description    :  设置SDA管脚为输入模式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sensor8_IIC_Set_SDA_Input(void)
{
    Sensor8_GPIO->CRL &= 0xFF0FFFFF;									// A组管脚 PIN5[23:20]
    Sensor8_GPIO->CRL |= 8 << 20;										// 设为上拉输入模式
}
// End of void Sensor8_IIC_Set_SDA_Input(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_Set_SDA_Output
* Description    :  设置SDA管脚为输出模式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sensor8_IIC_Set_SDA_Output(void)
{
     Sensor8_GPIO->CRL &= 0xFF0FFFFF;									// A组管脚 PIN5[23:20]
   	 Sensor8_GPIO->CRL |= 3 << 20;										// 设为推挽输出模式 
}
// End of void Sensor8_IIC_Set_SDA_Output(void)

/*******************************************************************************
//*                           王宇@2017-03-21
//* Function Name  :  Sensor8_IIC_Set_SCL_Output
//* Description    :  设置SCL管脚为输出模式
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
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_SDA_Read
* Description    :  读取SDA管脚电平
* Input          :  None
* Output         :  None
* Return         :  高电平返回1 低电平返回0
*******************************************************************************/
u8 Sensor8_IIC_SDA_Read(void)
{
    return(GPIO_ReadInputDataBit(Sensor8_GPIO, Sensor8_IIC_SDA));
    
}
// End of u8 Sensor8_IIC_SDA_Read(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_SCL_Read
* Description    :  读取SCL管脚电平
* Input          :  None
* Output         :  None
* Return         :  高电平返回1 低电平返回0
*******************************************************************************/
u8 Sensor8_IIC_SCL_Read(void)
{
    return(GPIO_ReadInputDataBit(Sensor8_GPIO, Sensor8_IIC_SCL));
    
}
// End of u8 Sensor8_IIC_SCL_Read(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_Port_Init
* Description    :  SHT30端口初始化
* Input          :  None
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 Sensor8_IIC_Init(void)
{
    u8 Transfer_Succeeded; 

	Sensor8_IIC_Pin_Init();													// 初始化管脚
	
	// 配置结构体
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
    
    Transfer_Succeeded = SW_IIC_Init(&Sensor8_IIC_s);							// 初始化IIC通信
    
    return (Transfer_Succeeded);
       
}
// End of u8  Sensor8_IIC_Port_Init(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_Variable_Init
* Description    :  温湿度变量初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Sensor8_IIC_Variable_Init(void)
{
  
}// End of void Sensor8_IIC_Variable_Init(void)


/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_Read_Register
* Description    :  读寄存器
* Input          :  u8 Chip_Addr	芯片地址
*					u16 usRead_Addr 要读取的地址
*                   u8* pBuffer     缓存指针
*                   u16 usRead_Len  读取长度
* Output         :  None
* Return         :  1成功 0失败
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
*                           王宇@2017-03-21
* Function Name  :  Sensor8_IIC_Write_Register
* Description    :  写芯片寄存器
* Input          :  u16 usWrite_Addr    要写入的地址
*                   u8* pBuffer         缓存指针
*                   u16 usWrite_Len     写入长度 
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 Sensor8_IIC_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len)
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
// End of u8 Temp_Humi_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len)

/******************* (C) COPYRIGHT 2017 王宇 **************END OF FILE*********/

