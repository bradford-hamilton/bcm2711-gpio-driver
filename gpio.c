#include <stdio.h>
#include "gpio.h"

#define SYS_GPIO_CLASS "/sys/class/bcm2711-gpios"
#define HIGH "1"
#define LOW "0"
#define INPUT "in"
#define OUTPUT "out"

int gpio_configure_direction(char *label, uint8_t direction)
{
  char path[256];
  FILE *file;

  snprintf(path, sizeof(path), SYS_GPIO_CLASS "/%s/direction", label);

  file = fopen(path, "w");
  if (!file) {
    perror("Failed to open GPIO direction file");
    return -1;
  }

  if (direction) {
    fprintf(file, OUTPUT);
  } else {
    fprintf(file, INPUT);
  }

  fclose(file);

  return 0;
}

int gpio_write_value(char *label, uint8_t value)
{
  char path[256];
  FILE *file;

  snprintf(path, sizeof(path), SYS_GPIO_CLASS "/%s/value", label);

  file = fopen(path, "w");
  if (!file) {
    perror("Failed to open GPIO value file");
    return -1;
  }

  if (value) {
    fprintf(file, HIGH);
  } else {
    fprintf(file, LOW);
  }

  fclose(file);

  return 0;
}

int gpio_read_value(char *label)
{
  char path[256];
  FILE *file;
  int value;

  snprintf(path, sizeof(path), SYS_GPIO_CLASS "/%s/value", label);

  file = fopen(path, "r");
  if (!file) {
    perror("Failed to open GPIO value file");
    return -1;
  }

  if (fscanf(file, "%d", &value) != 1) {
    fclose(file);
    perror("Failed to read GPIO value");
    return -1;
  }

  return value;
}
