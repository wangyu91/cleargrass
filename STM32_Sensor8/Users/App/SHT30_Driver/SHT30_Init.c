/******************** (C) COPYRIGHT 2016 王宇 **********************************
* File Name          :  SHT30_Init.c
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  23/03/2017
* Description        :  SHT30温湿度传感器初始化程序
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "SHT30_Init.h"
#include "GPIO_LED.h"
#include "SW_IIC.h"


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
//void SHT30_Delay(void);                                            	// 延时
//void SHT30_Delay_us(void);                                          	// us延时
void SHT30_Pin_Init(void);                                          	// 管脚初始化
void SHT30_Set_SDA_High(void);                                          // 拉高数据线
void SHT30_Set_SDA_Low(void);                                           // 拉低数据线
void SHT30_Set_SCL_High(void);                                          // 拉高时钟
void SHT30_Set_SCL_Low(void);                                           // 拉低时钟
//void SHT30_Set_SDA_Input(void);                                     	// 设置SDA为输入模式
//void SHT30_Set_SDA_Output(void);                                    	// 设置SDA为输出模式
//void SHT30_Set_SCL_Output(void);                                    	// 设置SCL为输出模式
u8   SHT30_SDA_Read(void);                                          	// 读取SDA电平
u8   SHT30_SCL_Read(void);                                          	// 读取SCL电平

void SHT30_Variable_Init(void);                                     	// SHT30变量初始化
u8   SHT30_Init(void);                                         			// SHT30端口初始化    
u8 SHT30_Read_Register(u16 usRead_Addr, u8* pBuffer, u16 usRead_Len);// 读寄存器
u8 SHT30_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len);// 写寄存器

/* Private functions ---------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
//*                           王宇@2017-03-21
//* Function Name  :  SHT30_Delay
//* Description    :  SHT30延时
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
//*                           王宇@2017-03-21
//* Function Name  :  SHT30_Delay_us
//* Description    :  SHT30 us延时
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
*                           王宇@2017-03-21
* Function Name  :  SHT30_Pin_Init
* Description    :  管脚初始化                   
* Input          :  P0.12   -->   SHT30_SCL_PIN
*                   P0.13   -->   SHT30_SDA_PIN
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
void SHT30_Pin_Init(void)
{
    
    // 设置管脚为开漏模式
    GPIO_Pin_Config(RCC_APB2Periph_GPIOA, GPIO_Pin_2, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, GPIOA);	// SCL A2脚
    GPIO_Pin_Config(RCC_APB2Periph_GPIOA, GPIO_Pin_3, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, GPIOA);   // SDA A3脚
    
}
// End of u8  SHT30_Pin_Init(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  SHT30_SDA_High
* Description    :  SHT30拉高SDA
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
*                           王宇@2017-03-21
* Function Name  :  SHT30_SDA_Low
* Description    :  SHT30拉低SDA
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
*                           王宇@2017-03-21
* Function Name  :  SHT30_SCL_High
* Description    :  SHT30拉高SCL
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
*                           王宇@2017-03-21
* Function Name  :  SHT30_SCL_Low
* Description    :  SHT30拉低SCL
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
//*                           王宇@2017-03-21
//* Function Name  :  SHT30_Set_SDA_Input
//* Description    :  设置SDA管脚为输入模式
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
//*                           王宇@2017-03-21
//* Function Name  :  SHT30_Set_SDA_Output
//* Description    :  设置SDA管脚为输出模式
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
//*                           王宇@2017-03-21
//* Function Name  :  SHT30_Set_SCL_Output
//* Description    :  设置SCL管脚为输出模式
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
*                           王宇@2017-03-21
* Function Name  :  SHT30_SDA_Read
* Description    :  读取SDA管脚电平
* Input          :  None
* Output         :  None
* Return         :  高电平返回1 低电平返回0
*******************************************************************************/
u8 SHT30_SDA_Read(void)
{
    return(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3));
    
}
// End of u8 SHT30_SDA_Read(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  SHT30_SCL_Read
* Description    :  读取SCL管脚电平
* Input          :  None
* Output         :  None
* Return         :  高电平返回1 低电平返回0
*******************************************************************************/
u8 SHT30_SCL_Read(void)
{
    return(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2));
    
}
// End of u8 SHT30_SCL_Read(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  SHT30_Port_Init
* Description    :  SHT30端口初始化
* Input          :  None
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 SHT30_Init(void)
{
    u8 Transfer_Succeeded; 

	SHT30_Pin_Init();													// 初始化管脚
	
	// 配置结构体
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
    
    Transfer_Succeeded = SW_IIC_Init(&SHT30_s);							// 初始化IIC通信
    
    return (Transfer_Succeeded);
       
}
// End of u8  SHT30_Port_Init(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  SHT30_Variable_Init
* Description    :  温湿度变量初始化
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void SHT30_Variable_Init(void)
{

    
}// End of void SHT30_Variable_Init(void)


/*******************************************************************************
//*                           王宇@2017-03-21
//* Function Name  :  Temp_Humi_Start
//* Description    :  启动传感器并且配置
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
//        // 软复位模块
//        ucError = 0;

//        // 读取串号
//        ucError |= SHT2x_GetSerialNumber(Temp_Humi.SerialNumber);

//        // 获取用户寄存器值
//        ucError |= SHT2x_ReadUserRegister(&Temp_Humi.User_Register);  

//        // 配置为10bit湿度 13bit温度模式
//        Temp_Humi.User_Register = (Temp_Humi.User_Register & ~SHT2x_RES_MASK) | SHT2x_RES_8_12BIT;
//        ucError |= SHT2x_WriteUserRegister(&Temp_Humi.User_Register); 

//        if (ucRetry_Time)
//        {
//            ucRetry_Time--;
//        }
//        
//    }while((ucError != 0) && ucRetry_Time);

//    
//    // 成功初始化传感器
//    if (ucError == 0)
//    {
//        // 清空错误标志位

//        NRF_LOG_INFO("Init SHT30 Succrss!\r\n");


//    }
//    else
//    {
//        // 置位传感器错误标志位


//        NRF_LOG_ERROR("Init SHT30 Failed!\r\n");
//    
//    }
//    

//    
//}// End of void Temp_Humi_Start(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  SHT30_Read_Register
* Description    :  读寄存器
* Input          :  u16 usRead_Addr 要读取的地址
*                   u8* pBuffer     缓存指针
*                   u16 usRead_Len  读取长度
* Output         :  None
* Return         :  1成功 0失败
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
*                           王宇@2017-03-21
* Function Name  :  SHT30_Write_Register
* Description    :  写芯片寄存器
* Input          :  u16 usWrite_Addr    要写入的地址
*                   u8* pBuffer         缓存指针
*                   u16 usWrite_Len     写入长度 
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 SHT30_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len)
{
    unsigned char Transfer_Succeeded = 1;
    u8 ucWrite_Addr[2];

    ucWrite_Addr[0] = (usWrite_Addr >> 8) & 0xFF;
    ucWrite_Addr[1] = (usWrite_Addr >> 0) & 0xFF;
    
    // 发送写命令
    Transfer_Succeeded &= SW_IIC_Transfer(&SHT30_s, SHT30_WRITE_ADDR, ucWrite_Addr, 2, SW_IIC_DONT_SEND_STOP);
    
    // 长度保护
    if (usWrite_Len == 0)
    {
        return 0;
    }

    // 发送数据
    while (usWrite_Len-- && Transfer_Succeeded)
    {
        Transfer_Succeeded &= SW_IIC_Write_Byte(&SHT30_s, *pBuffer);
        pBuffer++;
    }


    // 发送停止位
    Transfer_Succeeded &= SW_IIC_Stop(&SHT30_s);

    STM32_Delay_us(100);
    
    return Transfer_Succeeded;
    
}
// End of u8 Temp_Humi_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len)

/******************* (C) COPYRIGHT 2017 王宇 **************END OF FILE*********/

