#ifndef __BSP_BEEP_H
#define	__BSP_BEEP_H

#include "stm32f10x.h"
//Ұ��STM32F1������ beep ������ PC0�˿�

#define BEEP_ON 	GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define BEEP_OFF 	GPIO_ResetBits(GPIOC,GPIO_Pin_0)

void Beep_Init(void);

#endif /* __LED_H */
