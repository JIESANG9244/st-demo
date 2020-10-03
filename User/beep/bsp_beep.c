#include "bsp_beep.h"
/********************************************************************************
  * @file    bsp_beep.c
  * @author  fire
  * @brief   beepӦ�ú����ӿ�
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
*******************************************************************************/
void Beep_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ��PORTB,PORTEʱ��

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó���������
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��

    BEEP_OFF;

}
