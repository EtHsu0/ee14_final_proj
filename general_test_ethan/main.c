#include "stm32l476xx.h"

#include "LED.h"
#include "UART.h"
#include "SysClock.h"

#include <string.h>

int hit = 0;

void EXTI_Init(void) {
	// Enable SYSCFG Clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	// Select PA.3 as the trigger source of EXTI 3
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	SYSCFG->EXTICR[0] &= ~(0x000F);

	// Enable rising edge trigger for EXTI 3
	// Rising trigger selection register (RSTR)
	// 0 = disablsed, 1 = enabled
	EXTI->RTSR1 |= EXTI_RTSR1_RT0;

	// Disable falling edge trigger for EXTI 3
	// Falling trigger selection register (FSTR)
	// 0 = disabled, 1 = enabled
	EXTI-> FTSR1 &= ~EXTI_FTSR1_FT0;

	// Enable EXIT 3 interrupt
	// Interrupt mask register: 0 = masked, 1 = unmasked
	// "Masked" means that processor ignores the correspobnding interrupt.
	EXTI->IMR1 |= EXTI_IMR1_IM0; // Enable EXTI Line3

	// Set EXTI 3 priority to 1
	NVIC_SetPriority(EXTI0_IRQn, 1);
	// Enable EXTI 3 interrupt
	NVIC_EnableIRQ(EXTI0_IRQn);

}


void EXTI0_IRQHandler(void) {
	// Check for EXTI 3 interrupt flag
	if ((EXTI->PR1 & EXTI_PR1_PIF0) == EXTI_PR1_PIF0) {
		// Toggle LED
		Red_LED_Toggle();
		hit = 1;
		
		// Clear Interrupt pending request
		EXTI->PR1 |= EXTI_PR1_PIF0;
	}
}



unsigned int wait_for_first_hit() {
	unsigned int counter = 0;
   // hit = 0;

	while(hit != 1) {
		counter++;
	}
	return counter;
}


char RxComByte = 0;
uint8_t buffer[BufferSize];
char str[] = "Enter a wager for this round:\r\n";

int main(void){
	char rxByte; 
	int a, n, i;
	float b;
	unsigned int seed;
	
	System_Clock_Init();
	LED_Init();

	UART2_Init();
	USART_Write(USART2, (uint8_t *)str, strlen(str));	
	EXTI_Init();
	seed = wait_for_first_hit();

	while(1) {
		USART_Write(USART2, (uint8_t *)"LED is Off\r\n\r\n", 16);
		rxByte = USART_Read(USART2);
		if (rxByte == 'N' || rxByte == 'n'){
			Red_LED_Off();
			USART_Write(USART2, (uint8_t *)"LED is Off\r\n\r\n", 16);
		}
		else if (rxByte == 'Y' || rxByte == 'y'){
			Red_LED_On();
			USART_Write(USART2, (uint8_t *)"LED is on\r\n\r\n", 15);
		}
		
	}
}


