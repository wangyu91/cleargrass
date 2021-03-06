/*
 * Copyright (c) 2016, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "sw_i2c_gpio.h"
#include "Sensirion_configuration.h"

/*
 * INSTRUCTIONS
 * ============
 *
 * Implement all functions where they are marked as IMPLEMENT.
 * Follow the function specification in the comments.
 *
 * We use the following names for the two I2C signal lines:
 * SCL for the clock line
 * SDA for the data line
 */

/**
 * Initialize all hard- and software components that are needed to set the
 * SDA and SCL pins.
 */
void sensirion_init_pins()
{
    // IMPLEMENT
    GPIO_Pin_Config(Sgpc10_IIC2_GPIO_RCC, Sgpc10_IIC2_SDA|Sgpc10_IIC2_SCL, GPIO_Mode_Out_OD, GPIO_Speed_50MHz, Sgpc10_GPIO2);   // SDA|A9脚 SCL|A10脚 开漏输出 速率50MHz
    GPIO_SetBits(Sgpc10_GPIO2, Sgpc10_IIC2_SDA);
    GPIO_SetBits(Sgpc10_GPIO2, Sgpc10_IIC2_SCL);
}

/**
 * Configure the SDA pin as an input. The pin should be either left floating
 * or pulled up to the supply voltage.
 */
void sensirion_SDA_in()
{
//    // IMPLEMENT
//    Sgpc10_GPIO2->CRL &= 0xFF0FFFFF;									// G组管脚 PIN15[7:4]
//    Sgpc10_GPIO2->CRL |= 8 << 20;										// 设为上拉输入模式
    GPIO_SetBits(Sgpc10_GPIO2, Sgpc10_IIC2_SDA);
}

/**
 * Configure the SDA pin as an output and drive it low. The pin must be pulled
 * to ground or set to logical false.
 */
void sensirion_SDA_out()
{
//    // IMPLEMENT
//    Sgpc10_GPIO2->CRL &= 0xFF0FFFFF;									// G组管脚 PIN15[7:4]
//   	Sgpc10_GPIO2->CRL |= 3 << 20;										// 设为推挽输出模式
   	GPIO_ResetBits(Sgpc10_GPIO2, Sgpc10_IIC2_SDA);
}

/**
 * Read the value of the SDA pin.
 * @returns 0 if the pin is low and 1 otherwise.
 */
uint8_t sensirion_SDA_read()
{
    // IMPLEMENT
    return(GPIO_ReadInputDataBit(Sgpc10_GPIO2, Sgpc10_IIC2_SDA));
}

/**
 * Configure the SCL pin as an input. The pin should be either left floating
 * or pulled up to the supply voltage.
 */
void sensirion_SCL_in()
{
    // IMPLEMENT
    GPIO_SetBits(Sgpc10_GPIO2, Sgpc10_IIC2_SCL);
}

/**
 * Configure the SCL pin as an output and drive it low. The pin must be pulled
 * to ground or set to logical false.
 */
void sensirion_SCL_out()
{
    // IMPLEMENT
    GPIO_ResetBits(Sgpc10_GPIO2, Sgpc10_IIC2_SCL);
}

/**
 * Read the value of the SCL pin.
 * @returns 0 if the pin is low and 1 otherwise.
 */
uint8_t sensirion_SCL_read()
{
    // IMPLEMENT
    return(GPIO_ReadInputDataBit(Sgpc10_GPIO2, Sgpc10_IIC2_SCL));
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(uint32_t useconds) {
	STM32_Delay_us(useconds);
    // IMPLEMENT
}
