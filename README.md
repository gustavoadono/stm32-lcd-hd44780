
# STM32-LCD-HAL-Driver

A professional HAL-based HD44780 LCD driver library for STM32 microcontrollers, featuring a flexible configuration system and robust error handling.

## Features

### Hardware Abstraction Layer (HAL) Integration
- Full STM32 HAL compatibility
- Low-level GPIO handling with HAL primitives
- Microsecond precision timing control

### Flexible Configuration System
- Structured configuration for pins, timing, and display parameters
- Support for both 16x2 and other HD44780-compatible displays
- Configurable cursor and display settings
- Dynamic display control during runtime

### Comprehensive LCD Control
- 4-bit mode operation
- Custom character creation (up to 8 patterns)
- Precise cursor positioning
- Display clearing and homing functions

### Error Handling
- Parameter validation
- Status return codes
- Safe initialization sequence

## API Reference

### Initialization and Configuration

```c
int lcd_init(const struct lcd_config *config);
```

Initializes the LCD with the provided configuration. Configuration includes:
- GPIO pin assignments
- Display parameters (lines, font size)
- Timing parameters
- Cursor settings

### Display Control

```c
int lcd_set_display(const struct lcd_display_config *config);
int lcd_clear(void);
int lcd_home(void);
```

### Cursor and Position Control

```c
int lcd_set_cursor(const struct lcd_position *position);
```

### Character and Text Operations

```c
int lcd_write_char(char c);
int lcd_write_string(const char *str);
int lcd_create_char(uint8_t location, const uint8_t pattern[8]);
```

## Usage Example

```c
#include "hd44780.h"

int main(void) {
    // Configure LCD
    struct lcd_config config = {
        .pins = {
            .rs = {GPIOA, GPIO_PIN_0},
            .en = {GPIOA, GPIO_PIN_1},
            .data = {
                {GPIOA, GPIO_PIN_2},
                {GPIOA, GPIO_PIN_3},
                {GPIOA, GPIO_PIN_4},
                {GPIOA, GPIO_PIN_5}
            }
        },
        .display = {
            .two_lines = true,
            .display_on = true,
            .cursor_on = true,
            .cursor_blink = false,
            .big_font = false
        },
        .timing = {
            .init_delay = 40000,
            .enable_pulse_us = 1,
            .cmd_delay_us = 50,
            .clear_delay_us = 2000
        }
    };

    // Initialize LCD
    if (lcd_init(&config) != LCD_SUCCESS) {
        Error_Handler();
    }

    // Write text
    struct lcd_position pos = {0, 0};
    lcd_set_cursor(&pos);
    lcd_write_string("Hello, World!");

    while (1) {
        // Main loop
    }
}
```

## Installation

Add the source files to your project:

```
├── Inc/
│   ├── hd44780.h
│   └── hd44780defs.h
└── Src/
    └── hd44780.c
```

1. Configure your STM32 project to use HAL.
2. Include the header files in your source code.
3. Configure the LCD pins according to your hardware setup.

## Configuration Structures

### LCD Configuration

```c
struct lcd_config {
    struct lcd_gpio_pins pins;
    struct lcd_display_config display;
    struct lcd_timing_config timing;
};
```

### GPIO Configuration

```c
struct lcd_gpio_config {
    GPIO_TypeDef *port;
    uint16_t pin;
};
```

### Display Configuration

```c
struct lcd_display_config {
    bool two_lines;
    bool display_on;
    bool cursor_on;
    bool cursor_blink;
    bool big_font;
};
```

## Return Codes
- `LCD_SUCCESS`: Operation completed successfully
- `LCD_ERR_PARAM`: Invalid parameter provided

Additional error codes are defined in `hd44780defs.h`.

## Example Codes
Located in /examples directory:

- static_message.c: Basic text display
- scrolling_message.c: Dynamic text rendering
- custom_character.c: Creating custom characters


## Technical Notes
- Uses 4-bit mode interface for reduced pin count
- Implements proper initialization sequence as per HD44780 datasheet
- Includes microsecond delay calibration for accurate timing
- Supports both 5x8 and 5x10 dot matrix characters
- Hardware independent delay implementation

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/NewFeature`)
3. Commit your changes (`git commit -m 'Add NewFeature'`)
4. Push to the branch (`git push origin feature/NewFeature`)
5. Open a Pull Request

For bug reports or feature requests, please use the Issues page.
