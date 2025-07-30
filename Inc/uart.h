#include "main.h"

#define COMMAND_NUMBER 20
#define COMMAND_LENGTH 40
// led_all_on\n
// led_all_off\n
volatile uint8_t rx_buff[COMMAND_NUMBER][COMMAND_LENGTH];   // uart로 부터 들어온 문자를 저장 하는 버퍼(변수)
volatile int rear=0;   // input index: HAL_UART_RxCpltCallback에서 집어 넣어 주는 index
volatile int front=0;  // ouput index
