#include "stm32l476xx.h"
#include "SysTick.h"
#include "LED.h"
#include "LCD.h"
#include "stdlib.h"

static uint8_t hit = 0;

void System_Clock_Init(void){
	
	RCC->CR |= RCC_CR_MSION; 
	
	// Select MSI as the clock source of System Clock
	RCC->CFGR &= ~RCC_CFGR_SW; 
	
	// Wait until MSI is ready
	while ((RCC->CR & RCC_CR_MSIRDY) == 0); 	
	
	// MSIRANGE can be modified when MSI is OFF (MSION=0) or when MSI is ready (MSIRDY=1). 
	RCC->CR &= ~RCC_CR_MSIRANGE; 
	RCC->CR |= RCC_CR_MSIRANGE_7;  // Select MSI 8 MHz	
 
	// The MSIRGSEL bit in RCC-CR select which MSIRANGE is used. 
	// If MSIRGSEL is 0, the MSIRANGE in RCC_CSR is used to select the MSI clock range.  (This is the default)
	// If MSIRGSEL is 1, the MSIRANGE in RCC_CR is used. 
	RCC->CR |= RCC_CR_MSIRGSEL; 
	
	// Enable MSI and wait until it's ready	
	while ((RCC->CR & RCC_CR_MSIRDY) == 0); 		
}

void Joypad_init(void) {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	GPIOA->MODER &= ~(3U);
	GPIOA->MODER |= 0U;
	
	GPIOA->MODER &= ~(3U << (2 * 3));
	GPIOA->MODER |= 0U << (2 * 3);

	GPIOA->OSPEEDR &= ~(3U << (2*0));
	GPIOA->OSPEEDR |= 3U << (2*0);

	GPIOA->OSPEEDR &= ~(3U << (2*3));
	GPIOA->OSPEEDR |= 3U << (2*3);
}

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

	uint8_t a,b,c;
	while(hit != 1) {
		counter++;
	}
	return counter;
}
int main(void){
	unsigned int seed;
	uint8_t a,b,c;
	
	System_Clock_Init();
	LED_Init();
	LCD_Initialization();
	SysTick_Initialize(1000);
	Joypad_init();
	EXTI_Init();

	seed = wait_for_first_hit();
	srand(seed);
	while(1) {
		a = rand() % 100;
		b = rand() % 100;
		c = rand() % 100;
		if (GPIOA->IDR & 1 == 1) {
			Green_LED_On();
		} else {
			Green_LED_Off();
		}
		LCD_DisplayTime(a,b,c);
		delay(2000);
	}
}
