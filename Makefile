obj-m += gpio-driver.o

PWD := $(CURDIR)

.PHONY: code-to-pi led-app lcd-app

code-to-pi:
	scp -r bcm2711-gpio.dtsi Makefile *.c *.h ubuntu@192.168.1.249:/home/ubuntu/bcm2711-gpio-driver

# From raspberry pi session
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

led-app:
	gcc -Wall -Wextra -o ledbyte led-byte-app.c gpio.c

lcd-app:
	gcc -Wall -Wextra -o lcd lcd-app.c lcd.c gpio.c
