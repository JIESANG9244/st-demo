#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"
#

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM1_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
void Timer4_Action(void);

#endif
