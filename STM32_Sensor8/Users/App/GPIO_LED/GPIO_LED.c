#include "Global.h"
#include "SW_IIC.h"
#include "GPIO_LED.h"

void LED_Pin_Init(void);
void LED_SDA_High(void);
void LED_SDA_Low(void);
void LED_SCL_High(void);
void LED_SCL_Low(void);
u8   LED_SDA_Read(void);
u8   LED_SCL_Read(void);
u8   LED_Init(void);

void LED_Pin_Init(void)
{
	// SDA->Pin_8 LED_3 | SCL->Pin_7 LED_2
	GPIO_Pin_Config(RCC_APB2Periph_GPIOF, GPIO_Pin_8, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOF);
	GPIO_Pin_Config(RCC_APB2Periph_GPIOF, GPIO_Pin_7, GPIO_Mode_Out_PP, GPIO_Speed_50MHz, GPIOF);
}

void LED_SDA_High(void)
{
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
}

void LED_SDA_Low(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_8);
}

void LED_SCL_High(void)
{
	GPIO_ResetBits(GPIOF, GPIO_Pin_7);
}

void LED_SCL_Low(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_7);
}

u8 LED_SDA_Read(void)
{
	return(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8));
}

u8 LED_SCL_Read(void)
{
	return(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_7));
}

u8 LED_Init(void)
{
	u8 Succeed;
	LED_Pin_Init();
	
	//LED_t.Pin_Init = LED_Pin_Init;
	LED_t.SDA_High = LED_SDA_High;
	LED_t.SDA_Low  = LED_SDA_Low;
	LED_t.SCL_High = LED_SCL_High;
	LED_t.SCL_Low  = LED_SCL_Low;
	LED_t.SDA_Read = LED_SDA_Read;
	LED_t.SCL_Read = LED_SCL_Read;
	LED_t.Wait_Scl_Free_Timeout = (5 * 1000);

	Succeed = SW_IIC_Init(&LED_t);

	return Succeed;
}