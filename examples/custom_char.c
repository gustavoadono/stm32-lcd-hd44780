#include "hd44780.h"

struct lcd_config config = {
    .pins = {
        .rs = {GPIOB, GPIO_PIN_3},
        .en = {GPIOA, GPIO_PIN_10},
        .data = {
            {GPIOB, GPIO_PIN_10},
            {GPIOB, GPIO_PIN_4},
            {GPIOB, GPIO_PIN_5},
            {GPIOA, GPIO_PIN_15}
        }
    },
    .timing = {
        .init_delay = 50000,
        .enable_pulse_us = 1,
        .cmd_delay_us = 50,
        .clear_delay_us = 2000
    },
    .display = {
        .cursor_on = false,
        .cursor_blink = false,
        .display_on = true,
        .two_lines = true,
        .big_font = false
    }
};

const uint8_t bell_pattern[8] = {
    0b00100,
    0b01110,
    0b01110,
    0b01110,
    0b11111,
    0b00000,
    0b00100,
    0b00000
};

const uint8_t speaker_pattern[8] = {
    0b00001,
    0b00011,
    0b01111,
    0b01111,
    0b01111,
    0b00011,
    0b00001,
    0b00000
};

int main(void) {
    HAL_Init();
    
    if (lcd_init(&config) != LCD_SUCCESS) {
        Error_Handler();
    }
    
    lcd_create_char(0, bell_pattern);
    lcd_create_char(1, speaker_pattern);
    
    lcd_set_cursor_xy(0, 0);
    lcd_write_string("Alarm ");
    lcd_write_char(0);
    
    lcd_set_cursor_xy(1, 0);
    lcd_write_string("Volume ");
    lcd_write_char(1);
    
    while(1) {}
}
