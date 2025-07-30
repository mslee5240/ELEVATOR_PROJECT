#include "buzzer.h"

bool buzzer_executed = false;  // 실행 여부 확인용

void noTone() //소리 안나게
{
     htim3.Instance->CCR1=0;
     osDelay(50);
}

void buzzer_elevator(void)
{
	uint16_t elevator_bell[] = { A5, A5, G5, Gb5 };
	uint16_t elevator_duration[] = { 1, 1, 1, 2};

	int divide_freq = 1600000;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) ;

    for (int i = 0; i < ARRAY_SIZE(elevator_bell); i++)
    {
		__HAL_TIM_SET_AUTORELOAD(&htim3, divide_freq / elevator_bell[i]);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, divide_freq / elevator_bell[i] / 2);
		osDelay(300 * elevator_duration[i]);
		noTone();
    }

    osDelay(1000);
}

void buzzer_floor_select(void) {
	uint16_t floor_select[] = { A5 };
	uint16_t floor_select_duration[] = { 1 };

	int divide_freq = 1600000;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1) ;

	for (int i = 0; i < ARRAY_SIZE(floor_select); i++)
	 {
		__HAL_TIM_SET_AUTORELOAD(&htim3, divide_freq / floor_select[i]);
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, divide_freq / floor_select[i] / 2);
		osDelay(300 * floor_select_duration[i]);
		noTone();
	 }

	osDelay(1000);
}

void check_and_run_buzzer(void) //엘레베이터 도착음
{
	if ((input_floor == current_floor) && !buzzer_executed)
	{
	 buzzer_elevator();  // 한 번 실행
	 buzzer_executed = true; // 실행했으므로 다시 실행되지 않도록 설정
	}
}
