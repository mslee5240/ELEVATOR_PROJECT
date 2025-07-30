#include "main.h"
#include "extern.h"	// for delay_us()

#define HIGH	1
#define LOW		0

#define I2C_LCD_PORT 			GPIOB
#define I2C_LCD_SCL_PIN 		8
#define I2C_LCD_SDA_PIN 		9

// GPIO Register Addresses
#define GPIOB_BASE      0x40020400
#define GPIOB_MODER     (*(uint32_t *)(GPIOB_BASE + 0x00))  // MODE
#define GPIOB_OTYPER    (*(uint32_t *)(GPIOB_BASE + 0x04))  // Output TYPE
#define GPIOB_OSPEEDR   (*(uint32_t *)(GPIOB_BASE + 0x08))  // Output SPEED
#define GPIOB_PUPDR     (*(uint32_t *)(GPIOB_BASE + 0x0C))  // Pull-Up Pull-Down
#define GPIOB_IDR       (*(uint32_t *)(GPIOB_BASE + 0x10))  // Input Data
#define GPIOB_ODR       (*(uint32_t *)(GPIOB_BASE + 0x14))  // Ouput Data


// Response
#define ACK		0
#define NACK	1

// GPIO Pin Control
void i2c_gpio_init(void);
void pin_control(int pin_num, int level);
void sda_read_mode(void);
void sda_output_mode(void);
uint8_t sda_read(void);

void i2c_start(void);
void i2c_stop(void);
void i2c_write_bit(uint8_t bit);
uint8_t i2c_read_bit(void);
uint8_t i2c_write_byte(uint8_t byte);
uint8_t i2c_read_byte(uint8_t ack);


