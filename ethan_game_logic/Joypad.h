#ifndef __JOYPAD_H
#define __JOYPAD_H

#include "stm32l476xx.h"
#include <stdint.h>
void Joypad_Init(void);
unsigned int wait_for_first_hit(void);


#endif 
