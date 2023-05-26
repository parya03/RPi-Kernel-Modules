# 1 - GPIO

This module allows GPIO pin 23 of the RPi to be controlled by writing either a 1 or 0 (with null terminator) to /dev/p78_gpio.

This can be done with the C program included (write_to_led.c).

## How it works

The way it does this is the interesting part, since I wasn't able to find other (easily accessible) examples that do this, but the code implements the "Platform Data" portion of this kernel doc page: https://www.kernel.org/doc/html/latest/driver-api/gpio/board.html

This means that no device tree modifications or overlays are needed.

However, in order to do this, the code accesses an instance of gpio_chip directly (pinctrl-bcm2835), which was found after much digging in the kernel source code itself. This might mean the code is less portable, and the instance of gpio_chip doesn't seem to be necessarily written by the RPi foundation themselves, so might not be specific to the RPi and might change on other versions of the RPi (this module was written for the RPi 3B). It may be possible to create a standalone instance of gpio_chip, which would allow this to be more directly controlled and application-specific.