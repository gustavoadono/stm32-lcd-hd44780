#include "hd44780.h"
#include <string.h>

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

const char *scroll_message = "STM32C0 LCD Driver - Scrolling Text Demo  ";

int main(void)
{
    // Initialize peripherals
    HAL_Init();

    // Initialize LCD with predefined configuration
    if (lcd_init(&config) != LCD_SUCCESS)
    {
        Error_Handler();
    }

    size_t message_length = strlen(scroll_message);
    size_t scroll_pos = 0;

    while (1)
    {
        // Clear display
        lcd_clear();

        // First line: scrolling text
        lcd_set_cursor_xy(0, 0);
        for (int i = 0; i < 16; i++)
        {
            lcd_write_char(scroll_message[(scroll_pos + i) % message_length]);
        }

        // Second line: static message
        lcd_set_cursor_xy(1, 0);
        lcd_write_string("NUCLEO-C031C6");

        // Update scroll position
        scroll_pos = (scroll_pos + 1) % message_length;

        // Delay to control scroll speed
        HAL_Delay(300);
    }
}