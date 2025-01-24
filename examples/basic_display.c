#include "hd44780.h"

// LCD Configuration for NUCLEO-C031C6
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

int main(void)
{
    // Initialize peripherals
    HAL_Init();

    // Initialize LCD with predefined configuration
    if (lcd_init(&config) != LCD_SUCCESS)
    {
        // Handle initialization error
        Error_Handler();
    }

    // Display messages on different lines
    lcd_set_cursor_xy(0, 0);
    lcd_write_string("NUCLEO-C031C6");

    lcd_set_cursor_xy(1, 0);
    lcd_write_string("LCD Test");

    while (1)
    {
        // Main application loop
    }
}