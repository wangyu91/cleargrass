/*********************** @copy right 王宇 **************************************
* 文件名: main
* 描述  : led 流水灯
* 工具  : IAR 7.8x
* 日期  : 20/03/2017
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "led.h"
#include "stm32f10x.h"

/* Private Variables ---------------------------------------------------------*/

/* Private Typedef -----------------------------------------------------------*/

/* Private Function Prototype ------------------------------------------------*/
void Delay(__IO u32 nCount);											// 时延函数

/* Private Functions ---------------------------------------------------------*/
void Delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
}

void main()
{
	LED_GPIO_Config();													// 配置LED管脚

	while(1)
	{
		LED1(LED_ON);													// 点亮LED1
		Delay(0xFFFEFF);												// 时延
		LED1(LED_OFF);													// 熄灭

		LED2(LED_ON);
		Delay(0xFFFFEF);
		LED2(LED_OFF);

		LED3(LED_ON);
		Delay(0x0FFFEF);
		LED3(LED_OFF);
	}
}
/********************** End of File *******************************************/