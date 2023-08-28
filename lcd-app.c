#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "gpio.h"
#include "lcd.h"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    printf("Usage: %s \"<message>\" - (maximum 32 characters)\n", argv[0]);
    return 1;
  }

  size_t msg_len = strlen(argv[1]);
  if (msg_len > 32) {
    fprintf(stderr, "Usage: please provide a message between 1-32 characters. You sent %ld characters\n", msg_len);
    return 1;
  }

  char message[33]; // 32 characters + 1 for null terminator
  strncpy(message, argv[1], 32);
  message[32] = '\0';

  lcd_init();
  lcd_display_clear();
  lcd_print_string(message);
  sleep(3);
  lcd_display_clear();
}
