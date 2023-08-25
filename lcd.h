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

void lcd_init(void);
void lcd_enable(void);
void lcd_send_command(uint8_t cmd);
void lcd_display_clear();
void lcd_display_return_home();
void lcd_print_char(uint8_t data);
void lcd_print_string(char *message);

#endif // LCD_H
