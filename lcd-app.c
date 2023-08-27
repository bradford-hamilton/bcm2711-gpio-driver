#include <stdlib.h>
#include "gpio.h"
#include "lcd.h"

int main()
{
  lcd_init();
  lcd_display_clear();
  lcd_print_string("foo bar baz");
}
