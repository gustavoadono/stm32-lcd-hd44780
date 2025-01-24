
/**
 * @file
 * @brief Internal definitions for LCD 16x2 Display Driver
 *
 * This file contains internal definitions and constants for the
 * LCD 16x2 character display driver.
 */

#ifndef HD44780_DEFS_H_
#define HD44780_DEFS_H_

/* LCD commands */
#define LCD_CMD_CLEAR           0x01
#define LCD_CMD_HOME            0x02
#define LCD_CMD_ENTRY_MODE      0x04
#define LCD_CMD_DISPLAY_CTRL    0x08
#define LCD_CMD_SHIFT           0x10
#define LCD_CMD_FUNCTION_SET    0x20
#define LCD_CMD_CGRAM_ADDR      0x40
#define LCD_CMD_DDRAM_ADDR      0x80

/* LCD flags */
#define LCD_DISPLAY_ON          0x04
#define LCD_DISPLAY_OFF         0x00
#define LCD_CURSOR_ON           0x02
#define LCD_CURSOR_OFF          0x00
#define LCD_BLINK_ON            0x01
#define LCD_BLINK_OFF           0x00
#define LCD_TWO_LINE            0x08
#define LCD_ONE_LINE            0x00
#define LCD_5x10_DOTS           0x04
#define LCD_5x8_DOTS            0x00

/* LCD dimensions */
#define LCD_ROWS                2
#define LCD_COLUMNS             16
#define LCD_ROW_OFFSET_0        0x00
#define LCD_ROW_OFFSET_1        0x40

#endif /* HD44780_DEFS_H_ */