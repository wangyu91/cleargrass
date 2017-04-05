/******************** (C) COPYRIGHT 2016 王宇 **********************************
* File Name          :  Sensor8_IIC_Init7.c
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  27/03/2017
* Description        :  一拖八IIC初始化
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Sensor8_IIC_Init7.h"
#include "SW_IIC.h"


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
//void Sensor8_IIC_Delay(void);                                            	// 延时
void Sensor8_IIC7_Delay_us(void);										// us延时
void Sensor8_IIC7_Pin_Init(void);                                       // 管脚初始化
void Sensor8_IIC7_Set_SDA_High(void);                                   // 拉高数据线
void Sensor8_IIC7_Set_SDA_Low(void);                                    // 拉低数据线
void Sensor8_IIC7_Set_SCL_High(void);                                   // 拉高时钟
void Sensor8_IIC7_Set_SCL_Low(void);                                    // 拉低时钟
void Sensor8_IIC7_Set_SDA_Input(void);                                 	// 设置SDA为输入模式
void Sensor8_IIC7_Set_SDA_Output(void);                                	// 设置SDA为输出模式
//void Sensor8_IIC_Set_SCL_Output(void);                                    // 设置SCL为输出模式
u8   Sensor8_IIC7_SDA_Read(void);                                     	// 读取SDA电平
u8   Sensor8_IIC7_SCL_Read(void);                                      	// 读取SCL电平

void Sensor8_IIC7_Variable_Init(void);                                 	// SHT30变量初始化
u8   Sensor8_IIC7_Init(void);                                  			// SHT30端口初始化    
u8   Sensor8_IIC7_Read_Register(SW_IIC_t* IIC_s, u8 Chip_Addr, u16 usRead_Addr, u8* pBuffer, u16 usRead_Len);// 读寄存器
//u8 Sensor8_IIC_Write_Register(u8 usWrite_Addr, u8* pBuffer, u8 usWrite_Len);// 写寄存器
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
void Sensor8_IIC7_Delay_us(void)
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
void Sensor8_IIC7_Pin_Init(void)
{
    
    // 设置管脚为开漏模式
    GPIO_Pin_Config(Sensor8_IIC7_GPIO_RCC, Sensor8_IIC7_SDA|Sensor8_IIC7_SCL, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, Sensor8_GPIO7);   // SDA|F0脚 SCL|F1脚 开漏输出 速率50MHz 
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
void Sensor8_IIC7_Set_SDA_High(void)
{
    GPIO_SetBits(Sensor8_GPIO7, Sensor8_IIC7_SDA);
    
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
void Sensor8_IIC7_Set_SDA_Low(void)
{
    GPIO_ResetBits(Sensor8_GPIO7, Sensor8_IIC7_SDA);
    
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
void Sensor8_IIC7_Set_SCL_High(void)
{
    GPIO_SetBits(Sensor8_GPIO7, Sensor8_IIC7_SCL);
    
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
void Sensor8_IIC7_Set_SCL_Low(void)
{
    GPIO_ResetBits(Sensor8_GPIO7, Sensor8_IIC7_SCL);
    
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
void Sensor8_IIC7_Set_SDA_Input(void)
{
//    Sensor8_GPIO7->CRL &= 0xFFFFFFF0;									// F组管脚 PIN0[3:0]
//    Sensor8_GPIO7->CRL |= 8 << 0;										// 设为上拉输入模式
    GPIO_SetBits(Sensor8_GPIO7, Sensor8_IIC7_SDA);
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
void Sensor8_IIC7_Set_SDA_Output(void)
{
//     Sensor8_GPIO7->CRL &= 0xFFFFFFF0;								// F组管脚 PIN0[3:0]
//   	 Sensor8_GPIO7->CRL |= 3 << 0;									// 设为推挽输出模式 
   	 GPIO_ResetBits(Sensor8_GPIO7, Sensor8_IIC7_SDA);
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
u8 Sensor8_IIC7_SDA_Read(void)
{
    return(GPIO_ReadInputDataBit(Sensor8_GPIO7, Sensor8_IIC7_SDA));
    
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
u8 Sensor8_IIC7_SCL_Read(void)
{
    return(GPIO_ReadInputDataBit(Sensor8_GPIO7, Sensor8_IIC7_SCL));
    
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
u8 Sensor8_IIC7_Init(void)
{
    u8 Transfer_Succeeded; 

	Sensor8_IIC7_Pin_Init();											// 初始化管脚
	
	// 配置结构体
//    Sensor8_IIC7_s.Dealy                 = Sensor8_IIC_Delay;
    Sensor8_IIC7_s.Delay_us              = Sensor8_IIC7_Delay_us;
//    Sensor8_IIC7_s.Pin_Init              = Sensor8_IIC_Pin_Init;
    Sensor8_IIC7_s.SDA_High              = Sensor8_IIC7_Set_SDA_High;
    Sensor8_IIC7_s.SDA_Low               = Sensor8_IIC7_Set_SDA_Low;
    Sensor8_IIC7_s.SCL_High              = Sensor8_IIC7_Set_SCL_High;
    Sensor8_IIC7_s.SCL_Low               = Sensor8_IIC7_Set_SCL_Low;
    Sensor8_IIC7_s.Set_SDA_Input         = Sensor8_IIC7_Set_SDA_Input; 
    Sensor8_IIC7_s.Set_SDA_Output        = Sensor8_IIC7_Set_SDA_Output; 
//    Sensor8_IIC7_s.Set_SCL_Output        = Sensor8_IIC_Set_SCL_Output;   
    Sensor8_IIC7_s.SDA_Read              = Sensor8_IIC7_SDA_Read; 
    Sensor8_IIC7_s.SCL_Read              = Sensor8_IIC7_SCL_Read;
    Sensor8_IIC7_s.Wait_Scl_Free_Timeout = SENSOR8_TIMEOUT;
    
    Transfer_Succeeded = SW_IIC_Init(&Sensor8_IIC7_s);					// 初始化IIC通信
    
    return (Transfer_Succeeded);
       
}
// End of u8  Sensor8_IIC_Port_Init(void)

/******************* (C) COPYRIGHT 2017 王宇 *****END OF FILE******************/

