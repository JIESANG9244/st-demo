/**
  ******************************************************************************
  * @file    main.c
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  ******************************************************************************
  * ���������ļ�����  START THE FILE
  ******************************************************************************
  */ 

#include "stm32f10x.h"
#include "bsp_led.h"  
#include "bsp_key.h" 
#include "bsp_beep.h"
#include "bsp_exit.h"
#include "timer.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include <stdio.h>
#include "bsp_test.h"
#include "STM32F103xxxx_USART.H"  
/******************************************************************************
END THE FILE
  * ���������ļ�����*********************************************************/ 

void HardWare_Iint(void)
{
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  EXTI_Key_Config();
  LED_Iint();
  Key_Iint();
  Beep_Init();
  //TIM4_Int_Init(9999,7199);
  ILI9341_Init();
  USART_Configure();
  
}
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */ 
int main(void)
{	
	/* Ӳ���˿ڳ�ʼ�� */
	HardWare_Iint();
  
  
//  ILI9341_GramScan ( 6 );
  while(1)
  
  {
  } 

}








