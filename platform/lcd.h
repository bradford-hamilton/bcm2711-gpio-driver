#ifndef LCD_H
#define LCD_H

#include <stdint.h>

// LCD pins and their corresponding GPIO
#define LCD_GPIO_RS "gpio6"
#define LCD_GPIO_RW "gpio5"
#define LCD_GPIO_E  "gpio25"
#define LCD_GPIO_D4 "gpio24"
#define LCD_GPIO_D5 "gpio23"
#define LCD_GPIO_D6 "gpio22"
#define LCD_GPIO_D7 "gpio27"

// Initializes by instruction for the LCD based on datasheet power up.
void lcd_init(void);

// Starts data read/write.
void lcd_enable(void);

// Sends a command to the LCD.
void lcd_send_command(uint8_t cmd);

// Clears entire display and sets DDRAM address 0 in address counter.
void lcd_display_clear();

// Returns the cursor to the home position (Address 0).
void lcd_display_return_home();

// Sends a character to the LCD for display.
void lcd_print_char(uint8_t data);

// Sends a string to the LCD for display
void lcd_print_string(char *message);

#endif // LCD_H
