#include "timer.h"
#include "extern.h"
// delay_us(10);
// 1MHz의 주파수가 TIM2에 공급
// T=1/f 1/1000000Hz ==> 0.000001sec(1us) 1개의 펄스 소요 시간
// 1us * 1000 ==> 1ms
void delay_us(int us)
{
	//1. timer 2번의 counter reset
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	//2. 사용자가 지정한 시간만큼 wait
	while (__HAL_TIM_GET_COUNTER(&htim2) < us)
			;  // no operation
}
