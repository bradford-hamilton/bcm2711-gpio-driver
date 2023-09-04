obj-m += gpio-driver.o

PWD := $(CURDIR)

.PHONY: code-to-pi led-app lcd-app indent

# https://www.gnu.org/software/indent/manual/indent.html
# The Linux style is used in the linux kernel code and drivers.
# Code generally has to follow the Linux coding style to be accepted.
# This style is equivalent to the following settings:
indent:
	gindent -nbad -bap -nbc -bbo -hnl -br -brs -c33 -cd33 -ncdb -ce -ci4 \
            -cli0 -d0 -di1 -nfc1 -i8 -ip0 -l80 -lp -npcs -nprs -npsl -sai \
            -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts8 -il1 gpio-driver.c

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
