#include "stm32l476xx.h"
#include "SysTick.h"
#include "LED.h"
#include "LCD.h"

#include "SysClock.h"
#include "UART.h"
#include <string.h>

char RxComByte = 0;
uint8_t buffer[BufferSize];
char str[] = "Enter a wager for this round:\r\n";


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
	char rxByte;
	int a, n, i;
	float b;
	
	//System Clock Initialization
	//LED Initialization
	//SysTick Initialization
	//delay of 1Sec
	//LED Toggle
	
	System_Clock_Init();
	LED_Init();
	LCD_Initialization();
	SysTick_Initialize(1000);
	Joypad_init();
	
	//
	
	

	while(1) {
		USART_Write(USART2, (uint8_t *)str, strlen(str));	
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


