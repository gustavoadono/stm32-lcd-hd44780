/**
 * @file
 * @brief Public API for LCD 16x2 Display Driver
 *
 * This file contains the public API for the LCD 16x2 character display driver.
 * The driver is designed for HD44780-compatible LCD displays operating in 4-bit mode
 * on STM32C0 microcontrollers.
 */

#ifndef HD44780_H_
#define HD44780_H_

#include "stm32c0xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief GPIO configuration structure for LCD pins
     */
    struct lcd_gpio_config
    {
        GPIO_TypeDef *port; /**< GPIO port */
        uint16_t pin;       /**< GPIO pin number */
    };

    /**
     * @brief LCD pin configuration structure
     */
    struct lcd_pins_config
    {
        struct lcd_gpio_config rs;      /**< Register select pin */
        struct lcd_gpio_config en;      /**< Enable pin */
        struct lcd_gpio_config data[4]; /**< Data pins (D4-D7) */
    };

    /**
     * @brief LCD timing configuration (microseconds)
     */
    struct lcd_timing_config
    {
        uint32_t init_delay;      /**< Power-on initialization delay */
        uint32_t enable_pulse_us; /**< Enable pulse width */
        uint32_t cmd_delay_us;    /**< Command delay */
        uint32_t clear_delay_us;  /**< Clear display delay */
    };

    /**
     * @brief LCD display configuration
     */
    struct lcd_display_config
    {
        bool cursor_on;    /**< Show cursor */
        bool cursor_blink; /**< Cursor blink */
        bool display_on;   /**< Display on/off */
        bool two_lines;    /**< Two line mode */
        bool big_font;     /**< 5x10 dots font (false for 5x8) */
    };

    /**
     * @brief LCD complete configuration structure
     */
    struct lcd_config
    {
        struct lcd_pins_config pins;       /**< Pin configuration */
        struct lcd_timing_config timing;   /**< Timing configuration */
        struct lcd_display_config display; /**< Display configuration */
    };

    /**
     * @brief LCD cursor position structure
     */
    struct lcd_position
    {
        uint8_t row;    /**< Row (0-1) */
        uint8_t column; /**< Column (0-15) */
    };

/**
 * @brief Error codes for LCD operations
 */
#define LCD_SUCCESS 0    /**< Operation completed successfully */
#define LCD_ERR_PARAM -1 /**< Invalid parameter provided */
#define LCD_ERR_BUSY -2  /**< LCD controller is busy */

    /**
     * @brief Initialize LCD with given configuration
     *
     * @param config Pointer to LCD configuration structure
     *
     * @retval LCD_SUCCESS If successful
     * @retval LCD_ERR_PARAM If invalid config parameters
     */
    int lcd_init(const struct lcd_config *config);

    /**
     * @brief Clear LCD display
     *
     * @retval LCD_SUCCESS If successful
     * @retval LCD_ERR_BUSY If LCD is busy
     */
    int lcd_clear(void);

    /**
     * @brief Return cursor to home position
     *
     * @retval LCD_SUCCESS If successful
     * @retval LCD_ERR_BUSY If LCD is busy
     */
    int lcd_home(void);

    /**
     * @brief Set cursor position using row and column coordinates
     *
     * @param row    Row number (0-1 for 16x2 LCD)
     * @param column Column number (0-15 for 16x2 LCD)
     *
     * @retval LCD_SUCCESS    If successful
     * @retval LCD_ERR_PARAM  If row or column values are out of valid range
     */
    int lcd_set_cursor_xy(uint8_t row, uint8_t column);
    /**
     * @brief Write single character to LCD
     *
     * @param c Character to write
     *
     * @retval LCD_SUCCESS If successful
     * @retval LCD_ERR_BUSY If LCD is busy
     */
    int lcd_write_char(char c);

    /**
     * @brief Write string to LCD
     *
     * @param str Null-terminated string to write
     *
     * @retval LCD_SUCCESS If successful
     * @retval LCD_ERR_PARAM If str is NULL
     */
    int lcd_write_string(const char *str);

    /**
     * @brief Create custom character
     *
     * @param location Character code (0-7)
     * @param pattern Character pattern (8 bytes)
     *
     * @retval LCD_SUCCESS If successful
     * @retval LCD_ERR_PARAM If location > 7 or pattern is NULL
     */
    int lcd_create_char(uint8_t location, const uint8_t pattern[8]);

    /**
     * @brief Set display properties
     *
     * @param config Pointer to display configuration structure
     *
     * @retval LCD_SUCCESS If successful
     * @retval LCD_ERR_PARAM If config is NULL
     */
    int lcd_set_display(const struct lcd_display_config *config);

#ifdef __cplusplus
}
#endif

#endif /* HD44780_H_ */
