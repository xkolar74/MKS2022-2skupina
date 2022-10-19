/**
 ******************************************************************************
 * @file           : sct.c
 * @author         : Ondrej Kolar (203252)
 * @brief          : SCT2024 library source file
 ******************************************************************************
 */

// Includes
#include "sct.h"

// Variable definitions
static const uint32_t reg_values[3][10] = {
	{
		//PCDE--------GFAB @ DIS1
		0b0111000000000111 << 16,
		0b0100000000000001 << 16,
		0b0011000000001011 << 16,
		0b0110000000001011 << 16,
		0b0100000000001101 << 16,
		0b0110000000001110 << 16,
		0b0111000000001110 << 16,
		0b0100000000000011 << 16,
		0b0111000000001111 << 16,
		0b0110000000001111 << 16,
	}, {
		//----PCDEGFAB---- @ DIS2
		0b0000011101110000 << 0,
		0b0000010000010000 << 0,
		0b0000001110110000 << 0,
		0b0000011010110000 << 0,
		0b0000010011010000 << 0,
		0b0000011011100000 << 0,
		0b0000011111100000 << 0,
		0b0000010000110000 << 0,
		0b0000011111110000 << 0,
		0b0000011011110000 << 0,
	}, {
		//PCDE--------GFAB @ DIS3
		0b0111000000000111 << 0,
		0b0100000000000001 << 0,
		0b0011000000001011 << 0,
		0b0110000000001011 << 0,
		0b0100000000001101 << 0,
		0b0110000000001110 << 0,
		0b0111000000001110 << 0,
		0b0100000000000011 << 0,
		0b0111000000001111 << 0,
		0b0110000000001111 << 0,
	},
};


// Function definitions
void sct_init(void){
	// Initialize pins and clear register
	sct_nla(0);
	sct_clk(0);
	sct_raw(0);
	sct_noe(0);
}

void sct_raw(uint32_t value){
	// Send raw bit sequence to registers
	for (uint8_t i = 0; i < 32; i++){
		sct_sdi(value%2);
		value = value>>1;
		sct_clk(1);
		sct_clk(0);
	}
	sct_nla(1);
	sct_nla(0);
}

void sct_leds(uint8_t value){
	// Turn on LEDs by given binary number
	sct_leds_value_dots(value, 0, 0);
}

void sct_value(uint16_t value){
	// Show decimal value on the display
	sct_leds_value_dots(0, value, 0);
}

void sct_value_dots(uint16_t value, uint8_t dots){
	// Show decimal value on the display and corresponding DP segments
	sct_leds_value_dots(0, value, dots);
}

void sct_leds_value(uint8_t leds, uint16_t value){
	// Turn on LEDs by given binary number and show decimal value
	sct_leds_value_dots(leds, value, 0);
}

// Function to handle all peripherals connected to shift registers
void sct_leds_value_dots(uint8_t leds, uint16_t value, uint8_t dots){
	uint8_t lval;
	uint32_t dot_mask = 0;

	// Reversing higher nibble (32104567->76543210) of LEDs value
	lval = (leds & 0xC0)>>2 | (leds & 0x30)<<2 | (leds & 0x0F);
	lval = (lval & 0xA0)>>1 | (lval & 0x50)<<1 | (lval & 0x0F);

	// Manually turning on DP bits based on given value
	if(dots%2) dot_mask |= 1<<(31-16-0);
	if((dots>>1)%2) dot_mask |= 1<<(31-16-4);
	if((dots>>2)%2) dot_mask |= 1<<31;

	// Combining all of requested bits to turn on and sending to register
	sct_raw(reg_values[0][(value/100)%10] | reg_values[1][(value/10)%10] |
			reg_values[2][value%10] | (lval<<20) | dot_mask);
}
