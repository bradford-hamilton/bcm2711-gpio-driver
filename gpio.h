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

// Configure the gpio pin's direction (input/output)
int gpio_configure_direction(char *label, gpio_direction_t direction);

// Write a value to a gpio pin (high/low)
int gpio_write_value(char *label, gpio_value_t value);

// Read the current value of a gpio pin
int gpio_read_value(char *label);

#endif // GPIO_H
