/**
 ******************************************************************************
 * @file           : sct.h
 * @author         : Ondrej Kolar (203252)
 * @brief          : SCT2024 library header file
 ******************************************************************************
 */

#ifndef SCT_H_
#define SCT_H_

// Includes
#include "main.h"

// Macros
#define sct_sdi(x) HAL_GPIO_WritePin(SCT_SDI_GPIO_Port, SCT_SDI_Pin, x);
#define sct_clk(x) HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, x);
#define sct_nla(x) HAL_GPIO_WritePin(SCT_nLA_GPIO_Port, SCT_nLA_Pin, x);
#define sct_noe(x) HAL_GPIO_WritePin(SCT_nOE_GPIO_Port, SCT_nOE_Pin, x);

// Variable declarations
static const uint32_t reg_values[3][10];

// Function declarations
void sct_init(void); // Init registers with zeros
void sct_raw(uint32_t value); // Send raw sequence to registers
void sct_leds(uint8_t value); // Send just LED bar value
void sct_value(uint16_t value); // Send just decimal value to show on 7segments
void sct_value_dots(uint16_t value, uint8_t dots); // Send decimal value to display and DPs
void sct_leds_value(uint8_t leds, uint16_t value); // Send LED bar value and value to display
void sct_leds_value_dots(uint8_t leds, uint16_t value, uint8_t dots); // Send everything


#endif /* SCT_H_ */
