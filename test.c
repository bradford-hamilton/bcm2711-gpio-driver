#include <unistd.h>
#include "gpio.h"

int main()
{
  gpio_configure_direction("gpio6", DIRECTION_OUTPUT);
  gpio_configure_direction("gpio5", DIRECTION_OUTPUT);
  gpio_configure_direction("gpio27", DIRECTION_OUTPUT);
  gpio_configure_direction("gpio25", DIRECTION_OUTPUT);
  gpio_configure_direction("gpio24", DIRECTION_OUTPUT);
  gpio_configure_direction("gpio23", DIRECTION_OUTPUT);
  gpio_configure_direction("gpio22", DIRECTION_OUTPUT);
  gpio_configure_direction("gpio17", DIRECTION_OUTPUT);

  sleep(2);

  gpio_write_value("gpio6", VALUE_HIGH);
  gpio_write_value("gpio5", VALUE_HIGH);
  gpio_write_value("gpio27", VALUE_HIGH);
  gpio_write_value("gpio25", VALUE_HIGH);
  gpio_write_value("gpio24", VALUE_HIGH);
  gpio_write_value("gpio23", VALUE_HIGH);
  gpio_write_value("gpio22", VALUE_HIGH);
  gpio_write_value("gpio17", VALUE_HIGH);

  sleep(2);

  gpio_write_value("gpio6", VALUE_LOW);
  gpio_write_value("gpio5", VALUE_LOW);
  gpio_write_value("gpio27", VALUE_LOW);
  gpio_write_value("gpio25", VALUE_LOW);
  gpio_write_value("gpio24", VALUE_LOW);
  gpio_write_value("gpio23", VALUE_LOW);
  gpio_write_value("gpio22", VALUE_LOW);
  gpio_write_value("gpio17", VALUE_LOW);
}
