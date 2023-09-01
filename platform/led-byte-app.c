#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "gpio.h"

static char *gpio_labels[] = {"gpio6", "gpio5", "gpio25", "gpio24", "gpio23", "gpio22", "gpio27", "gpio17"};

void configure_all_gpios(gpio_direction_t direction);
void display_byte(uint8_t num);
void clear_byte();

int main(int argc, char* argv[])
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <8-bit-number>\n", argv[0]);
    return 1;
  }

  configure_all_gpios(DIRECTION_OUTPUT);

  errno = 0;
  char *end_ptr;
  long num = strtol(argv[1], &end_ptr, 10);
  if (errno != 0 || *end_ptr != '\0' || num < 0 || num > 255) {
    fprintf(stderr, "Error: Invalid 8-bit number provided\n");
    return 1;
  }

  display_byte(num);
  sleep(3);
  clear_byte();
}

void configure_all_gpios(gpio_direction_t direction)
{
  size_t gpio_labels_len = sizeof(gpio_labels) / sizeof(gpio_labels[0]);

  for (int i = gpio_labels_len - 1; i >= 0; i--) {
    gpio_configure_direction(gpio_labels[i], direction);
  }
}

void display_byte(uint8_t num)
{
  size_t gpio_labels_len = sizeof(gpio_labels) / sizeof(gpio_labels[0]);
  
  for (int i = gpio_labels_len - 1; i >= 0; i--) {
    size_t bit = (num >> i) & 1;
    if (bit == 1) {
      gpio_write_value(gpio_labels[i], VALUE_HIGH);
    }
  }
}

void clear_byte()
{
  size_t gpio_labels_len = sizeof(gpio_labels) / sizeof(gpio_labels[0]);

  for (int i = gpio_labels_len - 1; i >= 0; i--) {
    gpio_write_value(gpio_labels[i], VALUE_LOW);
  }
}
