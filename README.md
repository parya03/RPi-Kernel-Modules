# RPi-Kernel-Modules
My attempt at writing kernel modules for a Raspberry Pi 3B

## How to build
Cross-compiling from a "normal" computer is recommended for speed but building on the Raspberry Pi itself is doable. I used Ubuntu 22.04.

Sync the RPi kernel submodule:
    git submodule init
    git submodule update

The last command is going to take a while to download (depending on your internet connection) and a few GB of space, since it is downloading the whole RPi kernel.

Then follow the instructions to cross-compile the kernel on the Raspberry Pi website: https://www.raspberrypi.com/documentation/computers/linux_kernel.html#cross-compiling-the-kernel

So for the RPi 3B (which I used), for example, I would do (after installing the dependencies):
    cd linux
    KERNEL=kernel7
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcm2709_defconfig
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules dtbs -j[# of threads]
Where <# of threads> is the number of threads on your system (without the []).
Note that you don't need to make the "zImage" target since we are only interested in building the modules. Also I built a 32-bit version since for some reason Raspbian was installed as 32-bit on my Pi and I didn't feel the need to correct that, but your case might be different.

Now you can build each module by setting your compiler and running make:
    export ARCH=[arm or arm64]
    export CROSS_COMPILE=[arm-linux-gnueabihf- or aarch64-linux-gnu-]
    cd [module directory]
    make

## Credit

Credit to Johannes 4GNU_Linux's Linux driver tutorials on YouTube for helping me along with the basis for kernel module development. The Linux Kernel docs themselves were also (obviously) a great help.