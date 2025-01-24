/**
 * @file
 * @brief LCD 16x2 Display Driver Implementation (HAL Compatible)
 *
 * This file provides an implementation of the LCD 16x2 character display driver.
 * The driver supports HD44780-compatible LCD displays operating in 4-bit mode
 * on STM32 microcontrollers using the HAL library.
 */

#include "hd44780.h"
#include "hd44780defs.h"

/* Static configuration storage */
static struct lcd_config current_config;

/* Private function prototypes */
static void lcd_gpio_write(const struct lcd_gpio_config *gpio, GPIO_PinState state);
static void lcd_write_4bits(uint8_t data);
static void lcd_write_byte(uint8_t data, bool is_cmd);
static void lcd_pulse_enable(void);
static void lcd_delay_us(uint32_t us);

/**
 * @brief Initializes the LCD with the provided configuration
 *
 * This function configures the GPIO pins and initializes the LCD
 * in 4-bit mode with the specified settings, including display control
 * and cursor settings.
 *
 * @param config Pointer to the configuration structure
 * @return LCD_SUCCESS if initialization was successful, LCD_ERR_PARAM if invalid parameters
 */
int lcd_init(const struct lcd_config *config)
{
	if (config == NULL)
	{
		return LCD_ERR_PARAM;
	}

	/* Store configuration */
	current_config = *config;

	/* Configure GPIO pins */
	GPIO_InitTypeDef gpio_init = { 0 };
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_LOW;

	/* Initialize RS pin */
	gpio_init.Pin = config->pins.rs.pin;
	HAL_GPIO_Init(config->pins.rs.port, &gpio_init);

	/* Initialize EN pin */
	gpio_init.Pin = config->pins.en.pin;
	HAL_GPIO_Init(config->pins.en.port, &gpio_init);

	/* Initialize Data pins */
	for (int i = 0; i < 4; i++)
	{
		gpio_init.Pin = config->pins.data[i].pin;
		HAL_GPIO_Init(config->pins.data[i].port, &gpio_init);
	}

	/* Wait for power-up */
	HAL_Delay(config->timing.init_delay / 1000U);

	/* Initialize in 4-bit mode */
	lcd_write_4bits(0x03);
	lcd_delay_us(4500);
	lcd_write_4bits(0x03);
	lcd_delay_us(4500);
	lcd_write_4bits(0x03);
	lcd_delay_us(150);
	lcd_write_4bits(0x02);

	/* Set function */
	uint8_t function = LCD_CMD_FUNCTION_SET;
	if (config->display.two_lines)
	{
		function |= LCD_TWO_LINE;
	}
	if (config->display.big_font)
	{
		function |= LCD_5x10_DOTS;
	}
	lcd_write_byte(function, true);

	/* Set display control */
	uint8_t display = LCD_CMD_DISPLAY_CTRL;
	if (config->display.display_on)
	{
		display |= LCD_DISPLAY_ON;
	}
	if (config->display.cursor_on)
	{
		display |= LCD_CURSOR_ON;
	}
	if (config->display.cursor_blink)
	{
		display |= LCD_BLINK_ON;
	}
	lcd_write_byte(display, true);

	/* Clear display */
	lcd_clear();

	return LCD_SUCCESS;
}

/**
 * @brief Moves the cursor to the home position
 *
 * This function moves the cursor to the first position (top-left corner)
 * of the LCD display.
 *
 * @return LCD_SUCCESS on successful execution
 */
int lcd_home(void)
{
	lcd_write_byte(LCD_CMD_HOME, true);
	lcd_delay_us(current_config.timing.cmd_delay_us);
	return LCD_SUCCESS;
}

/**
 * @brief Sets the cursor position on the LCD display
 *
 * This function sets the cursor to the specified row and column on the LCD.
 *
 * @param row Row position (0 or 1)
 * @param column Column position (0 to 15)
 * @return LCD_SUCCESS if the operation was successful, LCD_ERR_PARAM if invalid parameters
 */
int lcd_set_cursor_xy(uint8_t row, uint8_t column)
{
	if (row >= LCD_ROWS || column >= LCD_COLUMNS)
	{
		return LCD_ERR_PARAM;
	}

	uint8_t address;
	switch (row)
	{
	case 0:
		address = LCD_ROW_OFFSET_0 + column;
		break;
	case 1:
		address = LCD_ROW_OFFSET_1 + column;
		break;
	default:
		return LCD_ERR_PARAM;
	}

	lcd_write_byte(LCD_CMD_DDRAM_ADDR | address, true);
	return LCD_SUCCESS;
}

/**
 * @brief Creates a custom character in CGRAM
 *
 * This function allows the creation of a custom character to be used
 * on the LCD display. The custom character pattern is stored in CGRAM.
 *
 * @param location Location in CGRAM (0 to 7)
 * @param pattern Array of 8 bytes representing the character pattern
 * @return LCD_SUCCESS if successful, LCD_ERR_PARAM if invalid parameters
 */
int lcd_create_char(uint8_t location, const uint8_t pattern[8])
{
	if (location > 7 || pattern == NULL)
	{
		return LCD_ERR_PARAM;
	}

	// Set CGRAM address
	lcd_write_byte(LCD_CMD_CGRAM_ADDR | (location << 3), true);

	// Write pattern
	for (int i = 0; i < 8; i++)
	{
		lcd_write_byte(pattern[i], false);
	}

	// Return to DDRAM mode
	lcd_write_byte(LCD_CMD_DDRAM_ADDR, true);
	return LCD_SUCCESS;
}

/**
 * @brief Configures the LCD display settings
 *
 * This function allows modification of display settings such as turning
 * the display on or off, enabling the cursor, and enabling the cursor
 * blink.
 *
 * @param config Pointer to the display configuration structure
 * @return LCD_SUCCESS if successful, LCD_ERR_PARAM if invalid parameters
 */
int lcd_set_display(const struct lcd_display_config *config)
{
	if (config == NULL)
	{
		return LCD_ERR_PARAM;
	}

	uint8_t display = LCD_CMD_DISPLAY_CTRL;
	if (config->display_on)
	{
		display |= LCD_DISPLAY_ON;
	}
	if (config->cursor_on)
	{
		display |= LCD_CURSOR_ON;
	}
	if (config->cursor_blink)
	{
		display |= LCD_BLINK_ON;
	}

	lcd_write_byte(display, true);
	current_config.display = *config;
	return LCD_SUCCESS;
}

/**
 * @brief Clears the LCD display
 *
 * This function clears all content from the LCD display and moves the
 * cursor to the home position.
 *
 * @return LCD_SUCCESS if the operation was successful
 */
int lcd_clear(void)
{
	lcd_write_byte(LCD_CMD_CLEAR, true);
	lcd_delay_us(current_config.timing.clear_delay_us);
	return LCD_SUCCESS;
}

/**
 * @brief Writes a single character to the LCD
 *
 * This function writes a single character to the LCD at the current
 * cursor position.
 *
 * @param c Character to be written to the display
 * @return LCD_SUCCESS if the operation was successful
 */
int lcd_write_char(char c)
{
	lcd_write_byte((uint8_t) c, false);
	return LCD_SUCCESS;
}

/**
 * @brief Writes a string to the LCD
 *
 * This function writes a string to the LCD, character by character.
 *
 * @param str Pointer to the string to be written
 * @return LCD_SUCCESS if the operation was successful, LCD_ERR_PARAM if the string is NULL
 */
int lcd_write_string(const char *str)
{
	if (str == NULL)
	{
		return LCD_ERR_PARAM;
	}

	while (*str)
	{
		lcd_write_char(*str++);
	}
	return LCD_SUCCESS;
}

/* Private functions */

/**
 * @brief Writes a single bit to a GPIO pin
 *
 * This function writes a specific state (SET or RESET) to a GPIO pin
 * for controlling the LCD.
 *
 * @param gpio Pointer to the GPIO configuration structure
 * @param state State to write to the pin (GPIO_PIN_SET or GPIO_PIN_RESET)
 */
static void lcd_gpio_write(const struct lcd_gpio_config *gpio, GPIO_PinState state)
{
	HAL_GPIO_WritePin(gpio->port, gpio->pin, state);
}

/**
 * @brief Sends a 4-bit nibble to the LCD
 *
 * This function sends a 4-bit data nibble to the LCD display by setting
 * the appropriate GPIO pins and pulsing the enable pin.
 *
 * @param data 4-bit data to be sent to the LCD
 */
static void lcd_write_4bits(uint8_t data)
{
	for (int i = 0; i < 4; i++)
	{
		lcd_gpio_write(&current_config.pins.data[i], (data >> i) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
	lcd_pulse_enable();
}

/**
 * @brief Sends a byte to the LCD
 *
 * This function sends a full byte (high and low nibbles) to the LCD.
 * It distinguishes between commands and data by using the RS pin.
 *
 * @param data Byte to be sent to the LCD
 * @param is_cmd Flag indicating whether the byte is a command (true) or data (false)
 */
static void lcd_write_byte(uint8_t data, bool is_cmd)
{
	lcd_gpio_write(&current_config.pins.rs, is_cmd ? GPIO_PIN_RESET : GPIO_PIN_SET);

	/* Send high nibble */
	lcd_write_4bits(data >> 4);

	/* Send low nibble */
	lcd_write_4bits(data & 0x0F);

	lcd_delay_us(current_config.timing.cmd_delay_us);
}

/**
 * @brief Pulses the enable pin to latch the data
 *
 * This function pulses the enable pin to latch data sent to the LCD
 * by toggling the enable pin state.
 */
static void lcd_pulse_enable(void)
{
	lcd_gpio_write(&current_config.pins.en, GPIO_PIN_RESET);
	lcd_delay_us(1);
	lcd_gpio_write(&current_config.pins.en, GPIO_PIN_SET);
	lcd_delay_us(current_config.timing.enable_pulse_us);
	lcd_gpio_write(&current_config.pins.en, GPIO_PIN_RESET);
}

/**
 * @brief Delays for a specified number of microseconds
 *
 * This function introduces a delay of a specified duration, in microseconds.
 *
 * @param us Number of microseconds to delay
 */
static void lcd_delay_us(uint32_t us)
{
	uint32_t count = (SystemCoreClock / 1000000U) * us / 5;
	while (count--)
	{
		__NOP();
	}
}
