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

const uint8_t battery_empty[8] = {
    0b01110,
    0b11011,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    0b11111
};

const uint8_t battery_quarter[8] = {
    0b01110,
    0b11011,
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    0b11111,
    0b11111
};

const uint8_t battery_half[8] = {
    0b01110,
    0b11011,
    0b10001,
    0b10001,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};

const uint8_t battery_full[8] = {
    0b01110,
    0b11011,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};

int main(void) {
    HAL_Init();
    
    if (lcd_init(&config) != LCD_SUCCESS) {
        Error_Handler();
    }
    
    while(1) {
        // Create battery stages
        lcd_create_char(0, battery_empty);
        lcd_set_cursor_xy(0, 0);
        lcd_write_string("Charging: ");
        lcd_write_char(0);
        HAL_Delay(1000);
        
        lcd_create_char(0, battery_quarter);
        lcd_set_cursor_xy(0, 0);
        lcd_write_string("Charging: ");
        lcd_write_char(0);
        HAL_Delay(1000);
        
        lcd_create_char(0, battery_half);
        lcd_set_cursor_xy(0, 0);
        lcd_write_string("Charging: ");
        lcd_write_char(0);
        HAL_Delay(1000);
        
        lcd_create_char(0, battery_full);
        lcd_set_cursor_xy(0, 0);
        lcd_write_string("Charged!  ");
        lcd_write_char(0);
        HAL_Delay(1000);
    }
}