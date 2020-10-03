#ifndef __BSP_TEST_H
#define	__BSP_TEST_H

// 第三方库函数测试 导入

#include "stm32f10x.h"
#include "bsp_led.h"  
#include "bsp_key.h" 
#include "bsp_beep.h"
#include "bsp_exit.h"
#include "timer.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "STM32F103xxxx_USART.H" 

// 第三方库函数 声明
void Printf_Charater(void);
void TFT_LCD_TEST(void);
static void Delay ( __IO uint32_t nCount );
void Usart_test(void); 
void LED_TEST(void);
void BSP_KEY1_EXIT_LCDTEST(void);
void BSP_KEY2_EXIT_LCDTEST(void);
void Beep_Test(void);
#endif
 
