/*********************** @copy right ���� **************************************
* �ļ���: main
* ����  : led ��ˮ��
* ����  : IAR 7.8x
* ����  : 20/03/2017
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "led.h"
#include "stm32f10x.h"

/* Private Variables ---------------------------------------------------------*/

/* Private Typedef -----------------------------------------------------------*/

/* Private Function Prototype ------------------------------------------------*/
void Delay(__IO u32 nCount);											// ʱ�Ӻ���

/* Private Functions ---------------------------------------------------------*/
void Delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
}

void main()
{
	LED_GPIO_Config();													// ����LED�ܽ�

	while(1)
	{
		LED1(LED_ON);													// ����LED1
		Delay(0xFFFEFF);												// ʱ��
		LED1(LED_OFF);													// Ϩ��

		LED2(LED_ON);
		Delay(0xFFFFEF);
		LED2(LED_OFF);

		LED3(LED_ON);
		Delay(0x0FFFEF);
		LED3(LED_OFF);
	}
}
/********************** End of File *******************************************/