#include "dotmatrix.h"

extern uint8_t stepmotor_state;
extern uint8_t current_floor;
extern uint8_t input_floor;

uint8_t one[] =     { 0x08, 0x18, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1c };
uint8_t two[] =     { 0x1c, 0x36, 0x22, 0x06, 0x0c, 0x18, 0x30, 0x3e };
uint8_t three[] =   { 0x38, 0x44, 0x04, 0x38, 0x04, 0x04, 0x44, 0x38 };
uint8_t four[] =    { 0x0c, 0x14, 0x24, 0x44, 0x7f, 0x04, 0x04, 0x04 };

uint8_t up_arrow[] = {
	0b00011000,
	0b00111100,
	0b01111110,
	0b01111110,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000
};

uint8_t down_arrow[] = {
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b01111110,
	0b01111110,
	0b00111100,
	0b00011000
};


unsigned char display_data[8];  			// 최종 8x8 출력할 데이터
unsigned char scroll_buffer[50][8] = { 0 }; // 스코롤할 데이타가 들어있는 버퍼
int number_of_character = 3;  				// 출력할 문자 갯수

uint8_t col[4] = { 0, 0, 0, 0 };

void dotmatrix_main(uint8_t pattern[]) { // 단일 배열 출력
	uint8_t temp;
	static int i = 0;

	col[0] = ~(1 << i);  // 00000001  --> 11111110
	col[1] = pattern[i];

	for (int j = 0; j < 2; j++) {
		for (int k = 0;k < 8; k++) {
			temp = col[j];
			if (temp & (1 << k)) {
				HAL_GPIO_WritePin(GPIOB, SER_74HC595_Pin, 1);
			}
			else {
				HAL_GPIO_WritePin(GPIOB, SER_74HC595_Pin, 0);
			}
			HAL_GPIO_WritePin(GPIOB, CLK_74HC595_Pin, 1);	//clk을 상승에서
			HAL_GPIO_WritePin(GPIOB, CLK_74HC595_Pin, 0);	//clk을 하강으로
		}
	}
	GPIOB->ODR &= ~GPIO_PIN_13;   // latch핀을 pull-down
	GPIOB->ODR |= GPIO_PIN_13;    // latch핀을 pull-up

	i++;
	i %= 8;
}

void floor_display() {
	switch(current_floor) {
        case 1:
            dotmatrix_main(one);
            break;

        case 2:
            dotmatrix_main(two);
            break;

        case 3:
            dotmatrix_main(three);
            break;

        case 4:
            dotmatrix_main(four);
            break;
	}
}

uint8_t blank[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t* up_arrows[] = { up_arrow };
uint8_t* down_arrows[] = { down_arrow };

void set_scroll_data(uint8_t** dis_data, int size) {
    // 전달받은 데이터(dis_data)를 scroll_buffer에 복사
    for (int i = 1; i < size + 1; i++) {
        for (int j = 0; j < 8; j++) {
            scroll_buffer[i][j] = dis_data[i - 1][j];
        }
    }
    // 스크롤의 끝에 빈 공간(blank) 추가
    for (int i = 0; i < 8; i++) {
        scroll_buffer[size + 1][i] = blank[i];
    }
}

void dotmatrix_print(void) {
	uint8_t temp;

	// idle
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_15, 0);

	for (int i = 0; i < 8; i++)
	{
		col[0] = ~(1 << i);  // 00000001  --> 11111110
		col[1] = display_data[i];

		for(int j = 0; j < 2; j++)
		{
			temp = col[j]; // 보낼 데이터 (1바이트)
			for(int i = 0; i < 8; i++)
				{
					if(temp & (1 << i))	{
						HAL_GPIO_WritePin(SER_74HC595_GPIO_Port, SER_74HC595_Pin, 1);
					} else {
						HAL_GPIO_WritePin(SER_74HC595_GPIO_Port, SER_74HC595_Pin, 0);
					}

					HAL_GPIO_WritePin(CLK_74HC595_GPIO_Port, CLK_74HC595_Pin, 1);
					HAL_GPIO_WritePin(CLK_74HC595_GPIO_Port, CLK_74HC595_Pin, 0);
				}
		}

		GPIOB->ODR &= ~GPIO_PIN_13;   // latch핀을 pull-down
		GPIOB->ODR |= GPIO_PIN_13;   // latch핀을 pull-up
		HAL_Delay(1);
	}
}

void dotmatrix_down_scroll (void) {
	static int count = 0;  // 컬럼 count
	static int index = 0;  // scroll_buffer의 2차원 index값
	static uint32_t past_time = 0;  // 이전 tick값 저장

	uint32_t now = HAL_GetTick();  // 1ms
	// 1.처음시작시 past_time=0; now: 500 --> past_time=500
	if (now - past_time >= 100) // 500ms scroll
	{
		past_time = now;
		for (int i=0; i < 8; i++)
		{
			display_data[i] = scroll_buffer[index + ((i - count) < 0)][(8 - count + i) % 8];
		}
		if (++count == 8) // 8칼람을 다 처리 했으면 다음 scroll_buffer로 이동
		{
			count = 0;
			index++;  // 다음 scroll_buffer로 이동
			if (index == 1 + 1) index=0;
			// 11개의 문자를 다 처리 했으면 0번 scroll_buffer를 처리 하기위해 이동
		}
	}
	dotmatrix_print();
}

void dotmatrix_up_scroll(void) {
	static int count = 0;  // 컬럼 count
	static int index = 0;  // scroll_buffer의 2차원 index값
	static uint32_t past_time = 0;  // 이전 tick값 저장

	uint32_t now = HAL_GetTick();  // 1ms
	// 1.처음시작시 past_time=0; now: 500 --> past_time=500
	if (now - past_time >= 100) // 500ms scroll
	{
		past_time = now;
		for (int i = 0; i < 8; i++)
		{
			display_data[i] = scroll_buffer[index + (i + count) / 8][(i + count) % 8];
		}
		if (++count == 8) // 8칼람을 다 처리 했으면 다음 scroll_buffer로 이동
		{
			count = 0;
			index++;  // 다음 scroll_buffer로 이동
			if (index == 1 + 1) index = 0;
			// 11개의 문자를 다 처리 했으면 0번 scroll_buffer를 처리 하기위해 이동
		}
	}
	dotmatrix_print();
}

void dotmatrix_elevator(void) {
    switch(stepmotor_state) {
		case IDLE:
			floor_display();
			break;

		case FORWARD:
			set_scroll_data(up_arrows, 1);	// scroll_buffer에 up arrow 데이터를 로드
			dotmatrix_up_scroll();				// 500ms 주기로 스크롤 업데이트 후 출력
			break;

		case BACKWARD:
			set_scroll_data(down_arrows, 1);		// scroll_buffer에 down arrow 데이터를 로드
			dotmatrix_down_scroll();				// 500ms 주기로 스크롤 업데이트 후 출력
			break;

		default:
			break;
    }
}



