#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define VALUE_HIGH 1
#define VALUE_LOW 0

#define DIRECTION_OUTPUT 1
#define DIRECTION_INPUT 0

int gpio_configure_direction(char *label, uint8_t direction);
int gpio_write_value(char *label, uint8_t value);
int gpio_read_value(char *label);

#endif // GPIO_H
