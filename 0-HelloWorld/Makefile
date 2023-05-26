obj-m += helloworld.o

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(PWD)/../rpi-kernel/ M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean