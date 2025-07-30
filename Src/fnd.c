#include "main.h"
#include "extern.h"
#include "stepmotor.h"
#include "gpio_register.h"

#define FND_D1_PIN 		4
#define FND_D2_PIN 		5
#define FND_D3_PIN 		6
#define FND_D4_PIN 		7
#define FND_DIGIT_MASK 	((1 << FND_D1_PIN) | (1 << FND_D2_PIN) | (1 << FND_D3_PIN) | (1 << FND_D4_PIN))

#define FND_SER_PIN			12
#define FND_LATCH_PIN		11
#define FND_CLK_PIN			10

// 폰트 배열
uint8_t fnd_font_forward_circle_0[] = { ~0xf7, ~0xf7, ~0xf7, ~0xf7, ~0xf7, ~0xf7, ~0xf7, ~0xf7, ~0xf7, ~0xf6, ~0xf4, ~0xf0, ~0x0C };
uint8_t fnd_font_forward_circle_1[] = { ~0xff, ~0xf7, ~0xf7, ~0xf7, ~0xf7, ~0xf7, ~0xf7, ~0xf7, ~0xf6, ~0xf6, ~0xf6, ~0xf6, ~0xC0 };
uint8_t fnd_font_forward_circle_2[] = { ~0xff, ~0xff, ~0xf7, ~0xf7, ~0xf7, ~0xf7, ~0xf7, ~0xf6, ~0xf6, ~0xf6, ~0xf6, ~0xf6, ~0x07 };
uint8_t fnd_font_forward_circle_3[] = { ~0xff, ~0xff, ~0xff, ~0xf7, ~0xe7, ~0xc7, ~0xc6, ~0xc6, ~0xc6, ~0xc6, ~0xc6, ~0xc6, ~0x92 };

uint8_t fnd_font_stop[] = {0x6d, 0x78, 0x3f, 0x73}; // s t o p


void fnd_spi_protocol(uint8_t serial_data) {
	GPIOC_ODR &= ~((1 << FND_SER_PIN) | (1 << FND_LATCH_PIN) | (1 << FND_CLK_PIN));

	for (int i = 7; i >= 0; i--) {
		if (serial_data & (1 << i)) {
			GPIOC_ODR |=  (1 << FND_SER_PIN);
		} else {
			GPIOC_ODR &= ~(1 << FND_SER_PIN);
		}
		GPIOC_ODR |=  (1 << FND_CLK_PIN);
		GPIOC_ODR &= ~(1 << FND_CLK_PIN);
	}

	GPIOC_ODR &= ~(1 << FND_LATCH_PIN);
	GPIOC_ODR |=  (1 << FND_LATCH_PIN);
}


void fnd_display(uint8_t *font) {
	static int digit_select = 0;
	uint8_t segment_data = 0;

	switch (digit_select) {
		case 0:
			GPIOB_ODR = (GPIOB_ODR & ~FND_DIGIT_MASK) | ((1 << FND_D2_PIN) | (1 << FND_D3_PIN) | (1 << FND_D4_PIN));
			segment_data = font[0];
			break;
		case 1:
			GPIOB_ODR = (GPIOB_ODR & ~FND_DIGIT_MASK) | ((1 << FND_D1_PIN) | (1 << FND_D3_PIN) | (1 << FND_D4_PIN));
			segment_data = font[1];
			break;
		case 2:
			GPIOB_ODR = (GPIOB_ODR & ~FND_DIGIT_MASK) | ((1 << FND_D1_PIN) | (1 << FND_D2_PIN) | (1 << FND_D4_PIN));
			segment_data = font[2];
			break;
		case 3:
			GPIOB_ODR = (GPIOB_ODR & ~FND_DIGIT_MASK) | ((1 << FND_D1_PIN) | (1 << FND_D2_PIN) | (1 << FND_D3_PIN));
			segment_data = font[3];
			break;
	}

	fnd_spi_protocol(segment_data);
	digit_select = (digit_select + 1) % 4;
}


void fnd_display_animation() {
	static int digit_select = 0;
	uint8_t segment_data = 0;

	switch (digit_select) {
		case 0:
			GPIOB_ODR = (GPIOB_ODR & ~FND_DIGIT_MASK) | ((1 << FND_D2_PIN) | (1 << FND_D3_PIN) | (1 << FND_D4_PIN));
			segment_data = fnd_font_forward_circle_3[counter100ms % 13];
			break;
		case 1:
			GPIOB_ODR = (GPIOB_ODR & ~FND_DIGIT_MASK) | ((1 << FND_D1_PIN) | (1 << FND_D3_PIN) | (1 << FND_D4_PIN));
			segment_data = fnd_font_forward_circle_2[counter100ms % 13];
			break;
		case 2:
			GPIOB_ODR = (GPIOB_ODR & ~FND_DIGIT_MASK) | ((1 << FND_D1_PIN) | (1 << FND_D2_PIN) | (1 << FND_D4_PIN));
			segment_data = fnd_font_forward_circle_1[counter100ms % 13];
			break;
		case 3:
			GPIOB_ODR = (GPIOB_ODR & ~FND_DIGIT_MASK) | ((1 << FND_D1_PIN) | (1 << FND_D2_PIN) | (1 << FND_D3_PIN));
			segment_data = fnd_font_forward_circle_0[counter100ms % 13];
			break;
	}

	fnd_spi_protocol(segment_data);
	digit_select = (digit_select + 1) % 4;
}


void fnd_main(void) {
	switch (stepmotor_state) {
		case IDLE:
			fnd_display(fnd_font_stop);
			break;
		case FORWARD:
		case BACKWARD:
			fnd_display_animation();
			break;
	}
}
