#include "bsp_test.h"

/**
  * @brief  LED_TEST LCD测试函数
  * @param  none
  * @retval 无
 */
 
void LED_TEST(void)
{
   LED1_ON;
   Delay_ms(1000);
   LED1_OFF;
   Delay_ms(500);
}

void Beep_Test(void)
{
  BEEP_OFF;
  Delay_ms(2000);
  BEEP_ON;
  Delay_ms(500);
}

/**
  * @brief  BSP__EXIT_LCD_TEST LCD测试函数
  * @param  none
  * @retval 无
 */
void BSP_KEY1_EXIT_LCDTEST(void)
{
  LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,WHITE);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
  ILI9341_DispStringLine_EN(LINE(0),"2020-10-03 14:30 BSP_EXIT_TEST");
  ILI9341_DispStringLine_EN(LINE(1),"KEY1 EXIT TEST");
  LCD_SetTextColor(RED);
  ILI9341_DrawCircle(100,200,20,1);
}

void BSP_KEY2_EXIT_LCDTEST(void)
{
  LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,WHITE);
  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
  ILI9341_DispStringLine_EN(LINE(3),"2020-10-03 14:30 DELEHUB");
  ILI9341_DispStringLine_EN(LINE(4),"KEY2 EXIT TEST");
  LCD_SetTextColor(GREEN);
  ILI9341_DrawCircle(100,200,40,0);
}
/**
  * @brief  TFT_LCD_TEST LCD测试函数
  * @param  none
  * @retval 无
 */
  
void  TFT_LCD_TEST(void)
{
  

	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,WHITE);

  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
	/********显示字符串示例*******/
  ILI9341_DispStringLine_EN(LINE(0),"dele 3.2 inch LCD:");
  ILI9341_DispStringLine_EN(LINE(1),"Image resolution:240x320");
  ILI9341_DispStringLine_EN(LINE(2),"ILI9341 TFT_LCD_TEST driver ");
  ILI9341_DispStringLine_EN(LINE(3),"www.delehub.top By wangzixuan");
  ILI9341_DispStringLine_EN(LINE(4),"2020-10-02 17:15 TFT_LCD_TEST");
  
  LCD_SetFont(&Font8x16);
  /* 画圆 */
  
  LCD_ClearLine(LINE(5));	/* 清除单行文字 */
	LCD_SetTextColor(GREEN);
	
  ILI9341_DispStringLine_EN(LINE(5),"TFT_LCD_DrawCir_TEST :");

	LCD_SetTextColor(RED);
  ILI9341_DrawCircle(100,200,20,1);
	
	LCD_SetTextColor(GREEN);
  ILI9341_DrawCircle(100,200,40,0);
	
	LCD_SetTextColor(BLUE);
	ILI9341_DrawCircle(100,180,60,0);

  Delay(0xFFFFFF);
  
  ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */

}



void Usart_test(void)
{
  USART_SendString(USART1 , "2020-10-02 18:00 USART1 Test");
  USART_SendLine(USART1);
  USART_SendNumber_SignedInteger(USART1,1234,4);
  USART_SendLine(USART1);
  USART_SendNumber_UnsignedInteger(USART1,12445,5);
  USART_SendLine(USART1);
  USART_SendNumber_Float(USART1,1234.89,7,8);
  USART_SendLine(USART1);
  USART_SendString(USART1 , "www.delehub.top  By wang zi xuan ");
  USART_SendLine(USART1);

}
/**
  * @brief  简单延时函数
  * @param  nCount ：延时计数值
  * @retval 无
  */	
static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}










