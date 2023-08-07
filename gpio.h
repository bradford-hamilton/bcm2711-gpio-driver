#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

typedef enum {
  DIRECTION_INPUT = 0,
  DIRECTION_OUTPUT = 1,
} gpio_direction_t;

typedef enum {
  VALUE_LOW = 0,
  VALUE_HIGH = 1,
} gpio_value_t;

int gpio_configure_direction(char *label, gpio_direction_t direction);
int gpio_write_value(char *label, gpio_value_t value);
int gpio_read_value(char *label);

#endif // GPIO_H
