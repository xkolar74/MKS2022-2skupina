/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f0xx.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif


// Symbol definitions
#define LED_TIME_BLINK  300
#define LED_TIME_SHORT  100
#define LED_TIME_LONG   1000
#define BTN_SAMPL_TIME1 40
#define BTN_SAMPL_TIME2 5

// Global variables
volatile uint32_t Tick = 0;

// Function declarations
void blikac(void);
void tlacitka1(void);
void tlacitka2(void);


int main(void) {

	SysTick_Config(8000); // Enable SysTick timer

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN; // enable port clocks
	GPIOA->MODER |= GPIO_MODER_MODER4_0; // LED1 = PA4, output
	GPIOB->MODER |= GPIO_MODER_MODER0_0; // LED2 = PB0, output
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR0_0; // S2 = PC0, pullup
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR1_0; // S1 = PC1, pullup

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // enable SYSCFG clock
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC; // select PC0 for EXTI0
	EXTI->IMR |= EXTI_IMR_MR0; // mask
	EXTI->FTSR |= EXTI_FTSR_TR0; // trigger on falling edge
	//NVIC_EnableIRQ(EXTI0_1_IRQn); // enable EXTI0_1

	/* Loop forever */
	for (;;) {
		blikac();
		tlacitka1();
	}

}


void EXTI0_1_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR0) { // check line 0 has triggered the IT
		EXTI->PR |= EXTI_PR_PR0;  // clear the pending bit
		GPIOB->ODR ^= (1 << 0);   // toggle LED2
	}
}


void SysTick_Handler(void) {
	Tick++;
}


void blikac(void) {
	static uint32_t delay = 0;
	if (Tick > delay + LED_TIME_BLINK) {
		delay = Tick;
		GPIOA->ODR ^= (1<<4);
	}
}


void tlacitka1(void) {
	static uint32_t old_s1;
	static uint32_t old_s2;
	static uint32_t off_time;

	// Read current switch states
	uint32_t new_s1 = GPIOC->IDR & (1 << 1); //S1
	uint32_t new_s2 = GPIOC->IDR & (1 << 0); //S2

	static uint32_t delay = 0;
	if (Tick > delay + BTN_SAMPL_TIME1) {
		delay = Tick;

		if (old_s1 && !new_s1) { // falling edge S1
			off_time = Tick + LED_TIME_SHORT;
			GPIOB->BSRR = (1 << 0);
		}

		if (old_s2 && !new_s2) { // falling edge S2
			off_time = Tick + LED_TIME_LONG;
			GPIOB->BSRR = (1 << 0);
		}

		// Save values as old states for next iteration
		old_s1 = new_s1;
		old_s2 = new_s2;
	}

	if (Tick > off_time) {
		GPIOB->BRR = (1 << 0);
	}

}

void tlacitka2(void) {
	static uint16_t debounce1 = 0xFFFF;
	static uint16_t debounce2 = 0xFFFF;
	static uint32_t off_time;

	debounce1 <<= 1;
	debounce2 <<= 1;
	if (GPIOC->IDR & (1 << 1)) debounce1 |= 1; //S1
	if (GPIOC->IDR & (1 << 0)) debounce2 |= 1; //S2

	if (debounce1 == 0x8000) { //If falling edge on S1
		off_time = Tick + LED_TIME_SHORT;
		GPIOB->BSRR = (1 << 0);
	}
	if (debounce2 == 0x8000) { //If falling edge on S2
		off_time = Tick + LED_TIME_LONG;
		GPIOB->BSRR = (1 << 0);
	}

	if (Tick > off_time) GPIOB->BRR = (1 << 0);

}
