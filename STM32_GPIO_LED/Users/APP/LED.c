/*********************** @copy right 王宇 **************************************
* 文件名: led.c
* 描述  : led LED 初始化
* 工具  : IAR 7.8x
* 日期  : 20/03/2017
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "LED.h"

/* Private Variables ---------------------------------------------------------*/

/* Private Function Prototype ------------------------------------------------*/

/* Private Functions ---------------------------------------------------------*/
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_t1, GPIO_t2;									// 声明GPIO结构体

	// 开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);

	// 配置结构体 初始化管脚
	GPIO_t1.GPIO_Pin   = GPIO_Pin_0;
	GPIO_t1.GPIO_Mode  = GPIO_Mode_Out_PP;								// 设为推拉模式
	GPIO_t1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_t2.GPIO_Pin   = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_t2.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_t2.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_t1);											// 初始化结构体
	GPIO_Init(GPIOF, &GPIO_t2);

	GPIO_SetBits(GPIOB, GPIO_Pin_0);									// 写管脚
	GPIO_SetBits(GPIOF, GPIO_Pin_7 | GPIO_Pin_8);
}

/******************** End of File *********************************************/
