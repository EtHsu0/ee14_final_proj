#include "Joypad.h"

void Joypad_Init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    GPIOA->MODER &= ~(3U);
    GPIOA->MODER |= 0U;

    GPIOA->OSPEEDR &= ~(3U << (2*0));
    GPIOA->OSPEEDR |= 3U << (2*0);
}


unsigned int wait_for_first_hit() {
    uint8_t prev = 0;
    uint8_t curr, toggle;
    unsigned int counter = 0;
    while (1) {
        curr = GPIOA->IDR & 1U;
        toggle = (curr ^ prev) & curr;
        prev = curr;
        if (toggle == 1)
            break;
        counter++;
    }
    
	return counter;
}
