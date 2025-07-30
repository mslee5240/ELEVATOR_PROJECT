#include "main.h"       // for variable "IN1_Pin"
#include "extern.h"
#include "button.h"

#define IDLE		0
#define FORWARD 	1
#define BACKWARD	2

// GPIO Register Addresses
#define GPIOC_BASE      0x40020800
#define GPIOC_MODER     (*(uint32_t *)(GPIOC_BASE + 0x00))  // MODE
#define GPIOC_OTYPER    (*(uint32_t *)(GPIOC_BASE + 0x04))  // Output TYPE
#define GPIOC_OSPEEDR   (*(uint32_t *)(GPIOC_BASE + 0x08))  // Output SPEED
#define GPIOC_PUPDR     (*(uint32_t *)(GPIOC_BASE + 0x0C))  // Pull-Up Pull-Down
#define GPIOC_IDR       (*(uint32_t *)(GPIOC_BASE + 0x10))  // Input Data
#define GPIOC_ODR       (*(uint32_t *)(GPIOC_BASE + 0x14))  // Ouput Data

void set_rpm(int rpm);
void stepmotor_drive(void);			// Thread로 돌아가기 때문에 while문 제거
void elevator_btn_input(void);		// 버튼 입력
void elevator_input_process(void);	// 입력 처리
void elevator_main(void);			// Thread에 넣을 main 함수


