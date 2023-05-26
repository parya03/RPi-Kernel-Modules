#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio/consumer.h>
#include <linux/gpio/machine.h>
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/of_fdt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Me");
MODULE_DESCRIPTION("A simple hello world module");
MODULE_VERSION("0.01");

#define BCM2387_GPIO_BASE 0x3F200000

#define DRIVER_NAME "p78_gpio"
#define DRIVER_CLASS "p78ModuleClass"

#define LED_GPIO 23

static ssize_t p78_read(struct file *file, char __user *user, size_t size, loff_t *loff);
static ssize_t p78_write(struct file *file, const char *user, size_t size, loff_t *loff);
static int p78_open(struct inode *device_file, struct file *instance);
static int p78_close(struct inode *device_file, struct file *instance);

static dev_t p78_device_num;
static struct class *p78_class;
static struct cdev p78_device_cdev;
static struct device *p78_device;

static struct gpio_desc *led_gpio;

struct file_operations fops = {
    .read = p78_read,
    .write = p78_write,
    .open = p78_open,
    .release = p78_close
};

// static struct of_device_id p78_driver_ids[] = {
//     {
//         .compatible = "p78_device"
//     },
//     { /* Sentinel */ }
// };

// static struct platform_driver p78_driver = {
//     .probe = p78_probe,
//     .remove = p78_remove,
//     .driver = {
//         .name = "p78_device_driver",
//         .of_match_table = p78_driver_ids,
//     },
// };

static int p78_open(struct inode *device_file, struct file *instance) {
    printk("p78 - dev file opened\n");
    return 0;
}

static int p78_close(struct inode *device_file, struct file *instance) {
    printk("p78 - dev file closed\n");
    return 0;
}

static ssize_t p78_read(struct file *file, char __user *user, size_t size, loff_t *loff) {
    printk("p78 read\n");

    return size;
}

static ssize_t p78_write(struct file *file, const char *user, size_t size, loff_t *loff) {
    printk("p78 write\n");

    int data;

    if(size > 1) {
        printk("p78 write: Invalid size");
    }

    copy_from_user(&data, user, size);

    data = (data > 1) ? 1 : data;

    gpiod_set_value(led_gpio, data);

    return size;
}

static __u32 major_number;

static struct gpiod_lookup_table p78_gpios_table = {
    .dev_id = DRIVER_NAME,
    .table = {
        GPIO_LOOKUP("pinctrl-bcm2835", 23, "led", GPIO_ACTIVE_HIGH),
        { }, // Empty entry to define end
    }
};

static int __init my_init(void) {
    printk("p78 Module loaded\n");

    // Register in /dev with dynamically allocated major number
    int alloc_retval = alloc_chrdev_region(&p78_device_num, 0, 1, DRIVER_NAME);
    // major_number = register_chrdev(0, "p78_testmod", &fops);

    if(major_number < 0) {
        printk("Registering device failed with %d\n", alloc_retval);
        return alloc_retval;
    }

    printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", p78_device_num >> 20, p78_device_num && 0xfffff);

    // Create device class
    if((p78_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
        printk("p78 - Can't create device class\n");
    }

    // Create device file
    if((p78_device = device_create(p78_class, NULL, p78_device_num, NULL, DRIVER_NAME)) == NULL) {
        printk("p78 - Couldn't create device\n");
    }

    // Init device file
    cdev_init(&p78_device_cdev, &fops);

    // Register device to kernel
    if(cdev_add(&p78_device_cdev, p78_device_num, 1) == -1) {
        printk("p78 - Couldn't register device with kernel\n");
    }
    
    // Add GPIO lookup table to kernel
    gpiod_add_lookup_table(&p78_gpios_table);

    // Get GPIO - Set as active high
    led_gpio = gpiod_get(p78_device, "led", GPIOD_OUT_HIGH);

    if(IS_ERR(led_gpio)) {
        printk("p78: Unable to obtain GPIO pointer\n");
    }

    gpiod_set_value(led_gpio, 0); // Set LED to 0 initially

    printk("p78: GPIO pointer: %p\n", led_gpio);
    
    return 0;
}

static void __exit my_exit(void) {
    gpiod_remove_lookup_table(&p78_gpios_table);
    cdev_del(&p78_device_cdev);
    device_destroy(p78_class, p78_device_num);
    class_destroy(p78_class);
    unregister_chrdev_region(major_number, 1);
    printk(KERN_INFO "p78 module unloaded\n");
    return;
}

module_init(my_init);
module_exit(my_exit);