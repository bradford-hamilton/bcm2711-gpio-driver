obj-m += gpio-char-driver.o

PWD := $(CURDIR)

.PHONY: code-to-pi

code-to-pi:
	scp -r Makefile *.c ubuntu@192.168.1.249:/home/ubuntu/bcm2711-gpio-driver/char

# From raspberry pi session
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
