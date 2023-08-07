obj-m += gpio-driver.o

PWD := $(CURDIR)

.PHONY: code-to-pi

code-to-pi:
	scp -r gpio-driver.c bcm2711-gpio.dtsi gpio.h gpio.c test.c Makefile ubuntu@192.168.1.250:/home/ubuntu/bcm2711-gpio-driver

# From pi session
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

move-dtb:
	sudo cp bcm2711-overlay.dtbo /boot/firmware/overlays
