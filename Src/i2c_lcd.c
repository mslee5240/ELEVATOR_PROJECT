#include "i2c_lcd.h"

void lcd_command(uint8_t command) {
	uint8_t high_nibble, low_nibble;
	uint8_t i2c_buffer[4];

	high_nibble = command & 0xf0;
	low_nibble = (command<<4) & 0xf0;
	i2c_buffer[0] = high_nibble | 0x04 | 0x08; //en=1, rs=0, rw=0, backlight=1
	i2c_buffer[1] = high_nibble | 0x00 | 0x08; //en=0, rs=0, rw=0, backlight=1
	i2c_buffer[2] = low_nibble  | 0x04 | 0x08; //en=1, rs=0, rw=0, backlight=1
	i2c_buffer[3] = low_nibble  | 0x00 | 0x08; //en=0, rs=0, rw=0, backlight=1

	i2c_start();
	delay_us(1);
	i2c_write_byte(I2C_LCD_ADDR_WRITE);
	delay_us(1);
	i2c_write_byte(i2c_buffer[0]);
	delay_us(1);
	i2c_write_byte(i2c_buffer[1]);
	delay_us(1);
	i2c_write_byte(i2c_buffer[2]);
	delay_us(1);
	i2c_write_byte(i2c_buffer[3]);
	delay_us(1);
	i2c_stop();

    // LCD 명령 처리 시간 대기
    // Clear Display(0x01)나 Return Home(0x02)는 처리 시간이 길어 대략 2ms 대기
    if (command == 0x01 || command == 0x02) {
    	delay_us(2000);
    } else {
        // 그 외 명령은 일반적으로 50µs 정도면 충분
        delay_us(50);
    }
}

void lcd_data(uint8_t data) {
	uint8_t high_nibble, low_nibble;
	uint8_t i2c_buffer[4];

	high_nibble = data & 0xf0;
	low_nibble = (data<<4) & 0xf0;
	i2c_buffer[0] = high_nibble | 0x05 | 0x08; //en=1, rs=1, rw=0, backlight=1
	i2c_buffer[1] = high_nibble | 0x01 | 0x08; //en=0, rs=1, rw=0, backlight=1
	i2c_buffer[2] = low_nibble  | 0x05 | 0x08; //en=1, rs=1, rw=0, backlight=1
	i2c_buffer[3] = low_nibble  | 0x01 | 0x08; //en=0, rs=1, rw=0, backlight=1

	i2c_start();
	delay_us(1);

	i2c_write_byte(I2C_LCD_ADDR_WRITE);
	delay_us(1);

	i2c_write_byte(i2c_buffer[0]);
	delay_us(1);
	i2c_write_byte(i2c_buffer[1]);
	delay_us(1);
	i2c_write_byte(i2c_buffer[2]);
	delay_us(1);
	i2c_write_byte(i2c_buffer[3]);
	delay_us(1);
	i2c_stop();

}

void i2c_lcd_init(void) {
	lcd_command(0x33);
	lcd_command(0x32);
	lcd_command(0x28);			//Function Set 4-bit mode
	lcd_command(DISPLAY_ON);
	lcd_command(0x06);			//Entry mode set
	lcd_command(CLEAR_DISPLAY);
	HAL_Delay(2);
}

void lcd_string(uint8_t *str) {
	while (*str) {
		lcd_data(*str++);
	}
}

void move_cursor(uint8_t row, uint8_t column) {
	lcd_command(0x80 | row << 6 | column);
	return;
}

void i2c_lcd_main(void) {
	uint8_t value=0;

	while(1)
	{
		move_cursor(0,0);
		lcd_string("Hello World!!!");
		move_cursor(1,0);
		lcd_data(value + '0');
		value++;
		if(value>9)value=0;
		HAL_Delay(500);
	}
}

void i2c_lcd_os_main(void) {
	static uint8_t value=0;

	move_cursor(0,0);
	lcd_string("Hello World!!!");
	move_cursor(1,0);
	lcd_data(value + '0');
	value++;
	if(value>9)value=0;
}











