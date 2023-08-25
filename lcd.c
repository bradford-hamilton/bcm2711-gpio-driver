#include <unistd.h>
#include "gpio.h"
#include "lcd.h"

void lcd_init(void)
{
  usleep(40 * 1000);                        // Wait for more than 40 ms after VCC rises to 2.7 V
  gpio_write_value(LCD_GPIO_RS, VALUE_LOW); // Set RS == 0 for LCD command
  gpio_write_value(LCD_GPIO_RW, VALUE_LOW); // Set RW == 0 for write
  write_4_bits(0x3);                        // Set D7, D6, D5, D4 to 0011
  usleep(5 * 1000);                         // Wait for more than 4.1 ms
  write_4_bits(0x3);                        // Set D7, D6, D5, D4 to 0011
  usleep(105);                              // Wait for more than 100 µs
  write_4_bits(0x3);                        // Set D7, D6, D5, D4 to 0011
  write_4_bits(0x02);                       // Set D7, D6, D5, D4 to 0010
  lcd_send_command(0x28);                   // 2 lines, font size 5x8
  lcd_send_command(0xE);                    // Turn display and cursor on
  lcd_display_clear();                      // Clear the display
  lcd_send_command(0x6);                    // Set entry mode increment addr by one
}

// Writes 4 bits of data to D4, D5, D6, D7 lines for 4-bit mode.
static void write_4_bits(uint8_t data)
{
  gpio_write_value(LCD_GPIO_D4, (data >> 0) & 0x1);
  gpio_write_value(LCD_GPIO_D5, (data >> 1) & 0x1);
  gpio_write_value(LCD_GPIO_D6, (data >> 2) & 0x1);
  gpio_write_value(LCD_GPIO_D7, (data >> 3) & 0x1);
 
  lcd_enable();
}

void lcd_enable(void)
{
  gpio_write_value(LCD_GPIO_E, VALUE_LOW);
  usleep(1);
  gpio_write_value(LCD_GPIO_E, VALUE_HIGH);
  usleep(1);
  gpio_write_value(LCD_GPIO_E, VALUE_LOW);
  usleep(100);
}

void lcd_send_command(uint8_t cmd)
{
  gpio_write_value(LCD_GPIO_RS, VALUE_LOW); // Set RS == 0 for LCD command
  gpio_write_value(LCD_GPIO_RW, VALUE_LOW); // Set RW == 0 for write

  write_4_bits(cmd >> 4);                   // High nibble
  write_4_bits(cmd);                        // Low nibble
}

void lcd_print_char(uint8_t data)
{
  gpio_write_value(LCD_GPIO_RS, VALUE_HIGH); // Set RS == 1 for data
  gpio_write_value(LCD_GPIO_RW, VALUE_LOW);  // Set RW == 0 for write

  write_4_bits(data >> 4);                   // High nibble
  write_4_bits(data);                        // Low nibble
}

void lcd_print_string(char *message)
{
  do {
    lcd_print_char((uint8_t)*message++);
  } while (message != '\0');
}

void lcd_display_clear()
{
  lcd_send_command(0x1);
  usleep(1700); // Execution Time = 82µs - 1.64ms
}

void lcd_display_return_home()
{
  lcd_send_command(0x2);
  usleep(1700); // Execution Time = 40µs - 1.6ms
}
