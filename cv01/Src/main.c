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

#include "stm32f030x8.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void) {

	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER5_0;

	uint32_t code = 0b00000001010100111011101110010101;

	/* Loop forever */
	for (;;) {

		for (uint8_t ind = 0; ind < 32; ind++) {

			if (code & (1UL << ind)) {
				GPIOA->BSRR = (1 << 5);
			} else {
				GPIOA->BRR = (1 << 5);
			}

			for (volatile uint32_t i = 0; i < 100000; i++) {
			}

		}

	}
}

