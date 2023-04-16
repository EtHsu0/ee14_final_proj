#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "stm32l476xx.h"

void EXTI_Init(void);
unsigned int wait_for_first_hit();


#endif 