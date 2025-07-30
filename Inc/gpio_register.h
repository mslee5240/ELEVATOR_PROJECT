#ifndef INC_GPIO_REGISTER_H_
#define INC_GPIO_REGISTER_H_

#include "main.h"

// GPIO Register Addresses
#define GPIOA_BASE		0x40020000
#define GPIOB_BASE      0x40020400
#define GPIOC_BASE      0x40020800

// GPIOA
#define GPIOA_MODER     (*(uint32_t *)(GPIOA_BASE + 0x00))  // MODE
#define GPIOA_OTYPER    (*(uint32_t *)(GPIOA_BASE + 0x04))  // Output TYPE
#define GPIOA_OSPEEDR   (*(uint32_t *)(GPIOA_BASE + 0x08))  // Output SPEED
#define GPIOA_PUPDR     (*(uint32_t *)(GPIOA_BASE + 0x0C))  // Pull-Up Pull-Down
#define GPIOA_IDR       (*(uint32_t *)(GPIOA_BASE + 0x10))  // Input Data
#define GPIOA_ODR       (*(uint32_t *)(GPIOA_BASE + 0x14))  // Ouput Data

// GPIOB
#define GPIOB_MODER     (*(uint32_t *)(GPIOB_BASE + 0x00))  // MODE
#define GPIOB_OTYPER    (*(uint32_t *)(GPIOB_BASE + 0x04))  // Output TYPE
#define GPIOB_OSPEEDR   (*(uint32_t *)(GPIOB_BASE + 0x08))  // Output SPEED
#define GPIOB_PUPDR     (*(uint32_t *)(GPIOB_BASE + 0x0C))  // Pull-Up Pull-Down
#define GPIOB_IDR       (*(uint32_t *)(GPIOB_BASE + 0x10))  // Input Data
#define GPIOB_ODR       (*(uint32_t *)(GPIOB_BASE + 0x14))  // Ouput Data

// GPIOC
#define GPIOC_MODER     (*(uint32_t *)(GPIOC_BASE + 0x00))  // MODE
#define GPIOC_OTYPER    (*(uint32_t *)(GPIOC_BASE + 0x04))  // Output TYPE
#define GPIOC_OSPEEDR   (*(uint32_t *)(GPIOC_BASE + 0x08))  // Output SPEED
#define GPIOC_PUPDR     (*(uint32_t *)(GPIOC_BASE + 0x0C))  // Pull-Up Pull-Down
#define GPIOC_IDR       (*(uint32_t *)(GPIOC_BASE + 0x10))  // Input Data
#define GPIOC_ODR       (*(uint32_t *)(GPIOC_BASE + 0x14))  // Ouput Data

#endif /* INC_GPIO_REGISTER_H_ */
