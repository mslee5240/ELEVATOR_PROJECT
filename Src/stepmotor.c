#include "stepmotor.h"

#define DEFAULT_RPM		13

uint8_t stepmotor_state = IDLE;
uint8_t current_floor = 0;
uint8_t input_floor = 0;

//IN1_Pin : PC6
int one_two_phase[] = { 0b0001, 0b0011, 0b0010, 0b0110, 0b0100, 0b1100, 0b1000, 0b1001 };   // 1-2상여자방식
int two_phase[] = { 0b0011, 0b0110, 0b1100, 0b1001, 0b0011, 0b0110, 0b1100, 0b1001 };       // 2상여자방식

// rpm : 1 ~ 13
void set_rpm(int rpm) {
	// 예외 처리
	if (rpm < 1) { rpm = 1; }
	if (rpm > 13) { rpm = 13;}

	delay_us(60000000 / 4096 / rpm);
	// 최대 Speed 기준(13) : delay_us(1126);
}

void stepmotor_drive(void) {	// Thread로 돌아가기 때문에 while문 제거
	static int8_t step = 0;
	GPIOC_ODR = (two_phase[step] << 6);	// PC6

	if (stepmotor_state == FORWARD) {
		step++;
		step %= 8;
	} else {
		step--;
		if (step < 0) {
			step = 7;
		}
	}
}

// 버튼 입력
void elevator_btn_input(void) {
    if (get_button(GPIOC, BTN0_Pin, BTN0) == BUTTON_PRESS){
        input_floor = 1;
        buzzer_floor_select();
    }

    if (get_button(GPIOC, BTN1_Pin, BTN1)==BUTTON_PRESS){
        input_floor = 2;
        buzzer_floor_select();
    }

    if (get_button(GPIOC, BTN2_Pin, BTN2)==BUTTON_PRESS){
        input_floor = 3;
        buzzer_floor_select();
    }

    if (get_button(GPIOC, BTN3_Pin, BTN3)==BUTTON_PRESS){
        input_floor = 4;
        buzzer_floor_select();

    }
}

// 입력 처리
void elevator_input_process(void) {
	if (input_floor == current_floor) {
		stepmotor_state = IDLE;
	} else if (input_floor > current_floor) {
		stepmotor_state = FORWARD;
	} else {
		stepmotor_state = BACKWARD;
	}
}

// Thread에 넣을 main 함수
void elevator_main(void) {
    elevator_btn_input();
    elevator_input_process();

	if (stepmotor_state != IDLE) {
		stepmotor_drive();
		set_rpm(DEFAULT_RPM);
	}
}


