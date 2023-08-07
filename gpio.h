#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define SYS_GPIO_CLASS "sys/class/bcm2711-gpios"
#define GPIO_HIGH 1
#define GPIO_LOW 0
#define GPIO_OUTPUT 1
#define GPIO_INPUT 0

int gpio_configure_direction(char *label, uint8_t direction);
int gpio_write_value(char *label, uint8_t value);
int gpio_read_value(char *label);

#endif // GPIO_H
