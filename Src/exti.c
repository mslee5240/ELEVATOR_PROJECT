#include "exti.h"

#include <stdbool.h>
extern bool buzzer_executed;

// Photo 센서 감지시 interrupt 발생
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin) {
		case GPIO_PIN_0: // 1층
			if (current_floor != 1) {
				current_floor = 1;
			}
			if (input_floor == 1) {
				stepmotor_state = IDLE; // 모터 정지
				buzzer_executed = false; // 부저(엘레베이터 도착음) 다시 실행
			}
			break;

		case GPIO_PIN_1: // 2층
			if (current_floor != 2) {
				current_floor = 2;
			}
			if(input_floor == 2){
				stepmotor_state = IDLE; // 모터 정지
				buzzer_executed = false;
			}
			break;

		case GPIO_PIN_2: // 3층
			if (current_floor != 3) {
				current_floor = 3;
			}
			if(input_floor == 3){
				stepmotor_state = IDLE; // 모터 정지
				buzzer_executed = false;
			}
			break;

		case GPIO_PIN_3: //4층
			if (current_floor != 4) {
				current_floor = 4;
			}
			if(input_floor == 4) {
				stepmotor_state = IDLE; // 모터 정지
				buzzer_executed = false;
			}
			break;
	}
}
