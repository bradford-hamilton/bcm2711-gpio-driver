obj-m += gpio-driver.o

PWD := $(CURDIR)

.PHONY: code-to-pi dtb

code-to-pi: dtb
	scp -r gpio-driver.c bcm2711-overlay.dtbo Makefile ubuntu@192.168.1.249:/home/bcm2711-gpio-driver

dtb:
	dtc -@ -I dts -O dtb -o bcm2711-overlay.dtbo bcm2711-overlay.dts

# From pi session
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

move-dtb:
	sudo cp bcm2711-overlay.dtbo /boot/firmware/overlays

# echo "dtoverlay=bcm2711-overlay" | sudo tee -a /boot/firmware/usercfg.txt
