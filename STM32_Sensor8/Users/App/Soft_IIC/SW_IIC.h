/******************** (C) COPYRIGHT 2017 王宇 **********************************
* File Name          :  SW_IIC.h
* Author             :  王宇
* CPU Type           :  STM32f103
* IDE                :  IAR 7.8
* Version            :  V1.0
* Date               :  22/03/2017
* Description        :  SW_I2C header file
*******************************************************************************/

/* Ifndef --------------------------------------------------------------------*/
#ifndef __SW_IIC_H
#define __SW_IIC_H

/* Includes ------------------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define     SW_IIC_SEND_ACK             0x01                            // 发送应答
#define     SW_IIC_DONT_SEND_ACK        0x00                            // 不发送应答
#define     SW_IIC_NEED_SEND_STOP       1                               // 需要发送停止位
#define     SW_IIC_DONT_SEND_STOP       0                               // 无需发送停止位

#define		SW_IIC_Get_ACK				0x00							// 有应答
#define		SW_IIC_No_ACK				0x01							// 无应答

#define		IIC_DELAY_COUNT				0xFF							// 延时计数

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
    //void                (*Delay)(void);                                 // 延时
    void                (*Delay_us)(void);                              // us延时
    //void                (*Pin_Init)(void);                              // 管脚初始化
    void                (*SDA_High)(void);                              // 拉高SDA
    void                (*SDA_Low)(void);                               // 拉低SDA
    void                (*SCL_High)(void);                              // 拉高SCL
    void                (*SCL_Low)(void);                               // 拉低SCL                
    void                (*Set_SDA_Input)(void);                         // 设置数据位输入管脚
    void                (*Set_SDA_Output)(void);                        // 设置数据位输出管脚
    //void                (*Set_SCL_Output)(void);                        // 设置时钟位输出管脚
    unsigned char       (*SDA_Read)(void);                              // 读SDA管脚
    unsigned char       (*SCL_Read)(void);                              // 读SCL管脚  
    unsigned int        Wait_Scl_Free_Timeout;                          // 等待SCL被释放超时时间                             
    
} SW_IIC_t;

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
extern  unsigned char SW_IIC_Init(SW_IIC_t * SW_IIC_s);					// 管脚初始化
extern  unsigned char SW_IIC_Write_Byte(SW_IIC_t * SW_IIC_s, unsigned char ICC_Data);  // IIC写字节
extern  unsigned char SW_IIC_Read_Byte(SW_IIC_t * SW_IIC_s, unsigned char * Byte_Buff, unsigned char Need_Ack); // IIC读字节
extern  unsigned char SW_IIC_Start(SW_IIC_t * SW_IIC_s);      // 产生起始信号
extern  unsigned char SW_IIC_Stop(SW_IIC_t * SW_IIC_s);       // 产生停止信号
extern  unsigned char SW_IIC_Transfer(SW_IIC_t * SW_IIC_s, unsigned char Chip_Addr, unsigned char * Data, unsigned short Data_Len, unsigned char Stop_Condition); // 发送数据

#endif /* __SW_IIC_H */

/******************* (C) COPYRIGHT 2017 王宇 *****END OF FILE******************/
