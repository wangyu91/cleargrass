/******************** (C) COPYRIGHT 2016 王宇 **********************************
* File Name          :  Goose_IIC_Init.c
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  06/04/2017
* Description        :  Goose IIC 初始化
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "Goose_IIC_Init.h"
#include "SW_IIC.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Goose_IIC_Delay_us(void);											// us延时
void Goose_IIC_Pin_Init(void);                                      	// 管脚初始化
void Goose_IIC_Set_SDA_High(void);                                      // 拉高数据线
void Goose_IIC_Set_SDA_Low(void);                                       // 拉低数据线
void Goose_IIC_Set_SCL_High(void);                                      // 拉高时钟
void Goose_IIC_Set_SCL_Low(void);                                       // 拉低时钟
void Goose_IIC_Set_SDA_Input(void);                                  	// 设置SDA为输入模式
void Goose_IIC_Set_SDA_Output(void);                                 	// 设置SDA为输出模式
//void Goose_IIC_Set_SCL_Output(void);                               	// 设置SCL为输出模式
u8   Goose_IIC_SDA_Read(void);                                      	// 读取SDA电平
u8   Goose_IIC_SCL_Read(void);                                       	// 读取SCL电平

u8   Goose_IIC_Init(void);                                   			// Goose IIC端口初始化    
/* Private functions ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
//*                           王宇@2017-03-21
//* Function Name  :  Goose_IIC_Delay
//* Description    :  IIC 延时
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//void Goose_IIC_Delay(void)
//{
//    nrf_delay_us(4);
//    
//}// End of void Goose_IIC_Delay(void)

/*******************************************************************************
*                           王宇@2017-03-27
* Function Name  :  Goose_IIC_Delay_us
* Description    :  IIC us延时
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Delay_us(void)
{
    STM32_Delay_us(1);
    
}
// End of void Goose_IIC_Delay_us(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Goose_IIC_Pin_Init
* Description    :  管脚初始化                   
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Pin_Init(void)
{
    
    // 设置管脚为开漏模式
    GPIO_Pin_Config(Goose_IIC_GPIO_RCC, Goose_IIC_SDA|Goose_IIC_SCL, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, Goose_GPIO);   // SDA|A5脚 SCL|A4脚 开漏输出 速率50MHz 
   // GPIO_Pin_Config(Goose_IIC_GPIO_C, Goose_IIC_SCL, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, Goose_IIC_GPIO);	// SCL A6脚
}
// End of u8  Goose_IIC_Pin_Init(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Goose_IIC_SDA_High
* Description    :  Goose IIC拉高SDA
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Set_SDA_High(void)
{
    GPIO_SetBits(Goose_GPIO, Goose_IIC_SDA);
    
}
// End of void Goose_IIC_SDA_High(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Goose_IIC_SDA_Low
* Description    :  Goose IIC拉低SDA
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Set_SDA_Low(void)
{
    GPIO_ResetBits(Goose_GPIO, Goose_IIC_SDA);
    
}
// End of void Goose_IIC_SDA_Low(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Goose_IIC_SCL_High
* Description    :  Goose IIC拉高SCL
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Set_SCL_High(void)
{
    GPIO_SetBits(Goose_GPIO, Goose_IIC_SCL);
    
}
// End of void Goose_IIC_SCL_High(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Goose_IIC_SCL_Low
* Description    :  Goose IIC拉低SCL
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Set_SCL_Low(void)
{
    GPIO_ResetBits(Goose_GPIO, Goose_IIC_SCL);
    
}
// End of void Goose_IIC_SCL_Low(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Goose_IIC_Set_SDA_Input
* Description    :  设置SDA管脚为输入模式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Set_SDA_Input(void)
{
    GPIO_SetBits(Goose_GPIO, Goose_IIC_SDA);
}
// End of void Goose_IIC_Set_SDA_Input(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Goose_IIC_Set_SDA_Output
* Description    :  设置SDA管脚为输出模式
* Input          :  None
* Output         :  None
* Return         :  None
*******************************************************************************/
void Goose_IIC_Set_SDA_Output(void)
{
   	 GPIO_ResetBits(Goose_GPIO, Goose_IIC_SDA);
}
// End of void Goose_IIC_Set_SDA_Output(void)

/*******************************************************************************
//*                           王宇@2017-03-21
//* Function Name  :  Goose_IIC_Set_SCL_Output
//* Description    :  设置SCL管脚为输出模式
//* Input          :  None
//* Output         :  None
//* Return         :  None
//*******************************************************************************/
//void Goose_IIC_Set_SCL_Output(void)
//{
//    NRF_GPIO->DIRSET = (1UL << Goose_IIC_SCL_PIN);
//    
//}// End of void Goose_IIC_Set_SCL_Output(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Goose_IIC_SDA_Read
* Description    :  读取SDA管脚电平
* Input          :  None
* Output         :  None
* Return         :  高电平返回1 低电平返回0
*******************************************************************************/
u8 Goose_IIC_SDA_Read(void)
{
    return(GPIO_ReadInputDataBit(Goose_GPIO, Goose_IIC_SDA));
    
}
// End of u8 Goose_IIC_SDA_Read(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Goose_IIC_SCL_Read
* Description    :  读取SCL管脚电平
* Input          :  None
* Output         :  None
* Return         :  高电平返回1 低电平返回0
*******************************************************************************/
u8 Goose_IIC_SCL_Read(void)
{
    return(GPIO_ReadInputDataBit(Goose_GPIO, Goose_IIC_SCL));
    
}
// End of u8 Goose_IIC_SCL_Read(void)

/*******************************************************************************
*                           王宇@2017-03-21
* Function Name  :  Goose_IIC_Port_Init
* Description    :  Goose IIC端口初始化
* Input          :  None
* Output         :  None
* Return         :  1成功 0失败
*******************************************************************************/
u8 Goose_IIC_Init(void)
{
    u8 Transfer_Succeeded; 

	Goose_IIC_Pin_Init();												// 初始化管脚
	
	// 配置结构体
    Goose_IIC_s.Delay_us              = Goose_IIC_Delay_us;
    Goose_IIC_s.SDA_High              = Goose_IIC_Set_SDA_High;
    Goose_IIC_s.SDA_Low               = Goose_IIC_Set_SDA_Low;
    Goose_IIC_s.SCL_High              = Goose_IIC_Set_SCL_High;
    Goose_IIC_s.SCL_Low               = Goose_IIC_Set_SCL_Low;
    Goose_IIC_s.Set_SDA_Input         = Goose_IIC_Set_SDA_Input; 
    Goose_IIC_s.Set_SDA_Output        = Goose_IIC_Set_SDA_Output; 
//    Goose_IIC_s.Set_SCL_Output        = Goose_IIC_Set_SCL_Output;   
    Goose_IIC_s.SDA_Read              = Goose_IIC_SDA_Read; 
    Goose_IIC_s.SCL_Read              = Goose_IIC_SCL_Read;
    Goose_IIC_s.Wait_Scl_Free_Timeout = Goose_TIMEOUT;
    
    Transfer_Succeeded = SW_IIC_Init(&Goose_IIC_s);						// 初始化IIC通信
    
    return (Transfer_Succeeded);
       
}
// End of u8  Goose_IIC_Port_Init(void)

/******************* (C) COPYRIGHT 2017 王宇 **************END OF FILE*********/
