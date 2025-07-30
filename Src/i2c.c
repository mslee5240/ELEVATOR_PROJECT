#include "i2c.h"

 /*
  * I2C Standard Mode (100kHz) 주요 타이밍:
  * - t_SU;STA (Start Setup Time): 최소 4.7 µs → delay_us(5)
  * - t_HD;STA (Start Hold Time): 최소 4.0 µs → delay_us(5)
  * - t_HIGH (Clock High Time): 최소 4.0 µs → delay_us(5)
  * - t_LOW (Clock Low Time): 최소 4.7 µs → delay_us(5)
  * - 신호 안정화를 위한 추가 딜레이: delay_us(1)
  */

/* GPIO 초기화: SDA, SCL를 출력 및 오픈드레인으로 구성 */
void i2c_gpio_init(void) {
    // SDA 핀 설정: 출력 모드, 오픈드레인, 풀업/풀다운 해제
    GPIOB_MODER &= ~(0b11 << (I2C_LCD_SDA_PIN * 2));
    GPIOB_MODER |=  (0b01 << (I2C_LCD_SDA_PIN * 2));
    GPIOB_OTYPER |= (1 << I2C_LCD_SDA_PIN);      		// Open-drain 설정
    GPIOB_PUPDR &= ~(0b11 << (I2C_LCD_SDA_PIN * 2));

    // SCL 핀 설정: 출력 모드, 오픈드레인, 풀업/풀다운 해제
    GPIOB_MODER &= ~(0b11 << (I2C_LCD_SCL_PIN * 2));
    GPIOB_MODER |=  (0b01 << (I2C_LCD_SCL_PIN * 2));
    GPIOB_OTYPER |= (1 << I2C_LCD_SCL_PIN);      		// Open-drain 설정
    GPIOB_PUPDR &= ~(0b11 << (I2C_LCD_SCL_PIN * 2));
}

/* GPIO Pin Control */
void pin_control(int pin_num, int level) {
	if (level == HIGH) {
		// 핀을 HIGH로 설정
		GPIOB_ODR |= 1 << pin_num;
	} else {
		GPIOB_ODR &= ~(1 << pin_num);
	}
}

void sda_output_mode(void) {
    // SDA 핀을 출력 모드 (01)로 설정
    // 먼저 해당 비트를 클리어한 후, 출력 모드로 설정하여 정확한 설정을 보장
    GPIOB_MODER &= ~(0b11 << (I2C_LCD_SDA_PIN * 2));
    GPIOB_MODER |=  (0b01 << (I2C_LCD_SDA_PIN * 2));
    // 내부 풀업/풀다운 해제 (00)
    GPIOB_PUPDR &= ~(0b11 << (I2C_LCD_SDA_PIN * 2));
}

void sda_input_mode(void) {
    // SDA 핀을 입력 모드로 설정 (00)
    GPIOB_MODER &= ~(0b11 <<(I2C_LCD_SDA_PIN * 2));
    // 내부 풀업/풀다운 해제 (00)
    GPIOB_PUPDR &= ~(0b11 << (I2C_LCD_SDA_PIN *2));
}

uint8_t sda_read(void) {
    // SDA 핀의 현재 입력 상태를 0 또는 1로 정규화하여 반환
    return (GPIOB_IDR >> I2C_LCD_SDA_PIN) & 0x01;
}

/* I2C Protocol (Bit-Banging) */
void i2c_start() {
    // I2C 시작 조건을 생성
    // SDA와 SCL을 HIGH 상태에서 SDA를 LOW로 전환한 후 SCL을 LOW로 설정
    sda_output_mode();
    pin_control(I2C_LCD_SDA_PIN, HIGH);
    pin_control(I2C_LCD_SCL_PIN, HIGH);
    delay_us(5);    					// t_SU;STA: 최소 4.7µs
    pin_control(I2C_LCD_SDA_PIN, LOW);
    delay_us(5);    					// t_HD;STA: 최소 4.0µs
    pin_control(I2C_LCD_SCL_PIN, LOW);


}

void i2c_stop() {
    // I2C 종료 조건을 생성
    // SDA가 LOW에서 시작하여 SCL을 HIGH로 올린 후 SDA를 HIGH로 전환
    sda_output_mode();
    pin_control(I2C_LCD_SDA_PIN, LOW);
    pin_control(I2C_LCD_SCL_PIN, HIGH);
    delay_us(5);    					// Stop 조건 설정, 일반적으로 4µs 이상
    pin_control(I2C_LCD_SDA_PIN, HIGH);
    delay_us(5);    					// Bus free time
}

void i2c_write_bit(uint8_t bit) {
    // SDA에 원하는 bit를 출력한 후 SCL을 한 번 펄스하여 해당 비트를 전송
    if (bit) {
    	pin_control(I2C_LCD_SDA_PIN, HIGH);
    } else {
    	pin_control(I2C_LCD_SDA_PIN, LOW);
    }
    delay_us(1);    // 신호 안정화

    // 클럭 펄스 생성
    pin_control(I2C_LCD_SCL_PIN, HIGH);
    delay_us(5);    					// t_HIGH: 최소 4µs
    pin_control(I2C_LCD_SCL_PIN, LOW);
    delay_us(1);    					// 추가 홀드 타임
}

uint8_t i2c_read_bit(void) {
    // SDA를 입력 모드로 설정한 후, SCL 펄스 동안 SDA의 값을 읽어 반환
    sda_input_mode();
    delay_us(1);    // 라인 안정화

    pin_control(I2C_LCD_SCL_PIN, HIGH);
    delay_us(5);    // t_HIGH: 최소 4µs
    uint8_t bit = sda_read();   // SDA의 입력값 읽기
    pin_control(I2C_LCD_SCL_PIN, LOW);
    delay_us(1);

    sda_output_mode();

    return bit;
}

/*
 * 8bit를 MSB부터 하나씩 전송
 * 8bit 전송 후 슬레이브가 ACK(0)을 보냈는지 확인
 * ACK 미수신 시 최대 3회까지 재시도
 */
uint8_t i2c_write_byte(uint8_t byte) {
    const uint8_t MAX_RETRIES = 3;  // 재시도 횟수
    uint8_t ack = NACK;

    for (int attempt = 0; attempt < MAX_RETRIES; attempt++) {
        uint8_t temp = byte;

        // 8bit 전송
        for (int i = 0; i < 8; i++) {
            // MSB 전송 후 좌측 시프트
            i2c_write_bit(temp & 0x80);
            temp <<= 1;
        }

        // 슬레이브의 ACK 응답 확인
        sda_input_mode();
        pin_control(I2C_LCD_SCL_PIN, HIGH);
        delay_us(5);    // 슬레이브가 ACK를 내릴 시간 제공
        ack = sda_read();   // ACK: 0, NACK: 1
        pin_control(I2C_LCD_SCL_PIN, LOW);
        sda_output_mode();

        if (ack == ACK) {
            break;  // 정상 전송
        } else {
            // 실패 시, 복구를 위해 Stop 조건을 발생시키고 잠시 지연 후 재시도
            i2c_stop();
            delay_us(5);
            i2c_start();
        }
    }
    return ack; // 0이면 성공, 1이면 실패(최대 재시도에도 ACK 미수신)
}

uint8_t i2c_read_byte(uint8_t ack) {
    // 8bit를 수신하여 하나의 byte를 구성
    // 마지막에 호출자가 전달한 ack 값을 그대로 전송하여,
    // 추가 데이터 요청(ACK: 0) 또는 종료(NACK: 1)를 알림.
    uint8_t byte = 0;
    sda_input_mode();

    for (int i = 0; i < 8; i++) {
        byte <<= 1; // 새 bit를 LSB에 붙이기 전에 공간을 비워두기 위함.
        pin_control(I2C_LCD_SCL_PIN, HIGH);
        delay_us(5);    // t_HIGH: 최소 4µs
        if (sda_read()) {
            byte |= 1;
        }
        pin_control(I2C_LCD_SCL_PIN, LOW);
        delay_us(1);
    }

    sda_output_mode();
    // 호출자가 전달한 ack 값을 그대로 전송
    i2c_write_bit(ack);

    return byte;
}
