#include "stm32l476xx.h"
#include "SysTick.h"
#include "LED.h"
#include "LCD.h"
#include <time.h>
#include <stdlib.h>


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
	
	GPIOA->OSPEEDR &= ~(3U << (2*0));
	GPIOA->OSPEEDR |= 3U << (2*0);
}

int main(void){
	//System Clock Initialization
	//LED Initialization
	//SysTick Initialization
	//delay of 1Sec
	//LED Toggle
	uint8_t a = 15,b=15,c=15;
	System_Clock_Init();
	LED_Init();
	LCD_Initialization();
	SysTick_Initialize(1000);
	Joypad_init();
	//
	// srand(time(0));
	while(1) {
		a = rand() % 60;
		b = rand() % 60;
		c = rand() % 100;
		c = rand() % 100;
		LCD_DisplayTime(a,b,c);
		Red_LED_On();
		while(1);
	}
}
