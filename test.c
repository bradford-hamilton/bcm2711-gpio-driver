#include <unistd.h>
#include "gpio.h"

static char *gpio_labels[] = {"gpio6", "gpio5", "gpio25", "gpio24", "gpio23", "gpio22", "gpio27", "gpio17"};

void configure_leds();
void set_leds(size_t num);

int main()
{
  configure_leds();

  sleep(2);

  set_leds(255);
}

void configure_leds()
{
  size_t gpio_labels_len = sizeof(gpio_labels) / sizeof(gpio_labels[0]);
  for (int i = gpio_labels_len - 1; i >= 0; i--) {
    gpio_configure_direction(gpio_labels[i], DIRECTION_OUTPUT);
  }
}

void set_leds(size_t num)
{
  size_t gpio_labels_len = sizeof(gpio_labels) / sizeof(gpio_labels[0]);
  
  for (int i = gpio_labels_len - 1; i >= 0; i--) {
    size_t bit = (num >> i) & 1;
    if (bit == 1) {
      gpio_write_value(gpio_labels[i], VALUE_HIGH);
    }
  }

  sleep(5);

  for (int i = gpio_labels_len - 1; i >= 0; i--) {
    gpio_write_value(gpio_labels[i], VALUE_LOW);
  }
}
