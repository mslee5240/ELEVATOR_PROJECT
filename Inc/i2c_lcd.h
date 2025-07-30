#include "main.h"
#include "extern.h"
#include "i2c.h"
#include "stm32f4xx_hal.h"

#include <string.h>
#include <stdio.h>

// I2C_LCD Addresses
#define I2C_LCD_ADDR		0x27
// R/W 비트(쓰기:0, 읽기:1)
#define I2C_LCD_ADDR_WRITE 	((I2C_LCD_ADDR << 1) | 0)   // 0x4E
#define I2C_LCD_ADDR_READ	((I2C_LCD_ADDR << 1) | 1)	// 0x4F

#define BACKLIGHT_ON 		0x08

/* LCD command */
#define DISPLAY_ON 			0x0C
#define DISPLAY_OFF 		0x08
#define CLEAR_DISPLAY 		0x01  //Delay 2msec
#define RETURN_HOME 		0x02

void lcd_command(uint8_t command);
void lcd_data(uint8_t data);
void i2c_lcd_init(void);
void lcd_string(uint8_t *str);
void move_cursor(uint8_t row, uint8_t column);
void i2c_lcd_main(void);
void i2c_lcd_os_main(void);


