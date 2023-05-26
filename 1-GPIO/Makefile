obj-m += p78_gpio.o

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C /home/p7810456/rpi-3-kernel/ M=$(PWD) modules
	arm-linux-gnueabihf-gcc write_to_led.c -o write_to_led

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm write_to_led