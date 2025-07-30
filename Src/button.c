#include "button.h"

void init_button(void) {
	// GPIOC의 Pin 0, 1, 2, 3을 Pull-up으로 Set
//	GPIOC_PUPDR = 0x00000055;

	// 내부 풀업/풀다운 해제 (00)
	GPIOC_PUPDR &= ~( 0b01 << (BTN0 * 2) | 0b01 << (BTN1 * 2) | 0b01 << (BTN2 * 2) | 0b01 << (BTN3 * 2) );
	// 내부 풀업 설정 (01)
	GPIOC_PUPDR |= 0b01 << (BTN0 * 2) | 0b01 << (BTN1 * 2) | 0b01 << (BTN2 * 2) | 0b01 << (BTN3 * 2);

}

int get_button(GPIO_TypeDef* GPIO, int GPIO_Pin, int button_num)
{
    // 버튼의 이전 상태를 저장 (static을 사용하여 함수 호출 간 값 유지)
    static unsigned char button_status[] =
        { BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE };

    // 버튼 상태 읽기; [HAL 방식]
//    int current_state = HAL_GPIO_ReadPin(GPIO, GPIO_Pin);

    // 방법_1; [DMA 방식으로 대체]
    int current_state = (GPIOC_IDR & GPIO_Pin) ? BUTTON_RELEASE : BUTTON_PRESS;

    // 방법_2;
//    int current_state = (GPIOC_IDR & GPIO_Pin) >> button_num;

    // 버튼이 눌린 경우 (기존 상태는 RELEASE)
    if (current_state == BUTTON_PRESS && button_status[button_num] == BUTTON_RELEASE) // 버튼이 처음 눌려진 noise high 상태
    {
        HAL_Delay(30);	// 노이즈 필터링 (디바운싱)
        button_status[button_num] = BUTTON_PRESS;	// 버튼이 눌린 상태 저장; noise가 지나간 상태의 High 상태
        return BUTTON_RELEASE;	// 아직 완전히 눌렸다 떼어진 상태가 아님
    }
    // 버튼이 떼어진 경우 (기존 상태는 PRESS)
    else if (current_state == BUTTON_RELEASE && button_status[button_num] == BUTTON_PRESS)
    {
    	HAL_Delay(30);	// 노이즈 필터링 (디바운싱)
        button_status[button_num] = BUTTON_RELEASE;	// 버튼 상태 초기화
        return BUTTON_PRESS;	// 버튼이 눌렸다가 떼어진 상태 (완전한 입력)
    }

    return BUTTON_RELEASE;	// 버튼이 눌리지 않음 (idle 상태)
}

void button_led_toggle_test(void) {
	// 버튼을 한번 눌렀다 떼면 led가 toggle되도록 code를 구현
	if (get_button(GPIOC, GPIO_PIN_0, BTN0) == BUTTON_PRESS) {
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
	}

	if (get_button(GPIOC, GPIO_PIN_1, BTN1) == BUTTON_PRESS) {
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
	}

	if (get_button(GPIOC, GPIO_PIN_2, BTN2) == BUTTON_PRESS) {
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	}

	if (get_button(GPIOC, GPIO_PIN_3, BTN3) == BUTTON_PRESS) {
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
	}

	if (get_button(GPIOC, GPIO_PIN_13, BTN4) == BUTTON_PRESS) {	// Demo b'd
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
}



