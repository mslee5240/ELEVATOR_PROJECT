
// UART & printf Variable
extern uint8_t rx_data;   // uart2 rx byte
extern UART_HandleTypeDef huart2;
extern t_print o_prt;

// TIMER Variable
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;		// buzzer
extern volatile int TIM11_1ms_counter;
extern void delay_us(int us);
extern volatile int counter100ms;

// STEP_MOTOR Variable
extern uint8_t stepmotor_state;
extern uint8_t current_floor;
extern uint8_t input_floor;

// BUZZER Variable

/*--------------------------------------------------------------*/

// LED Func
extern void led_main(void);
extern void button_led_toggle_test(void);

// BTN Func
extern void init_button(void);
extern int get_button(GPIO_TypeDef* GPIO, int GPIO_Pin, int button_num);

// DS1302 Func
extern void init_gpio_ds1302();
extern void init_date_time(void);
extern void init_ds1302();
extern void ds1302_main(void);
extern void pc_command_processing(void);
extern void set_rtc(char *date_time);

// FLASH Func
extern void flash_main();
extern void flash_set_time(void);

// I2C_LCD Func
extern void lcd_command(uint8_t command);
extern void lcd_data(uint8_t data);
extern void i2c_lcd_init(void);
extern void lcd_string(uint8_t *str);
extern void move_cursor(uint8_t row, uint8_t column);
extern void i2c_lcd_main(void);
extern void i2c_lcd_os_main(void);

// DOT_MATRIX Func
extern void floor_display();
extern void dotmatrix_main(uint8_t pattern[]);
extern void dotmatrix_elevator(void);

// BUZZER Func
extern void buzzer_main();

// SERVO_MOTOR Func
extern void servo_motor_main(void);

// STEP_MOTOR Func
extern void elevator_main(void);			// Thread에 넣을 main 함수

// Buzzer Func
extern void buzzer_elevator(void);
extern void buzzer_floor_select(void);
extern void check_and_run_buzzer(void);

// Fnd Func
extern void fnd_main(void);



