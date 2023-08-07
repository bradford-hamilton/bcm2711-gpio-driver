obj-m += gpio-driver.o

PWD := $(CURDIR)

.PHONY: code-to-pi led-app

code-to-pi:
	scp -r gpio-driver.c bcm2711-gpio.dtsi gpio.h gpio.c led-byte.c Makefile ubuntu@192.168.1.250:/home/ubuntu/bcm2711-gpio-driver

# From raspberry pi session
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

led-app:
	gcc -Wall -Wextra -o main led-byte.c gpio.c
