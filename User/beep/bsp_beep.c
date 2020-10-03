#include "bsp_beep.h"
/********************************************************************************
  * @file    bsp_beep.c
  * @author  fire
  * @brief   beep应用函数接口
  * 实验平台:野火 F103-霸道 STM32 开发板 
*******************************************************************************/
void Beep_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能PORTB,PORTE时钟

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置成上拉输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化

    BEEP_OFF;

}
