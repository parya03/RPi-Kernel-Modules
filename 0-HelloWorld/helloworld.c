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
MODULE_AUTHOR("p7810456");
MODULE_DESCRIPTION("A simple hello world module");
MODULE_VERSION("0.01");

#define DRIVER_NAME "p78_testmod"
#define DRIVER_CLASS "p78ModuleClass"

static ssize_t p78_read(struct file *file, char __user *user, size_t size, loff_t *loff);
static ssize_t p78_write(struct file *file, const char *user, size_t size, loff_t *loff);
static int p78_open(struct inode *device_file, struct file *instance);
static int p78_close(struct inode *device_file, struct file *instance);

static dev_t p78_device_num;
static struct class *p78_class;
static struct cdev p78_device_cdev;
static struct device *p78_device;

static int major_number;

struct file_operations fops = {
    .read = p78_read,
    .write = p78_write,
    .open = p78_open,
    .release = p78_close
};

static int p78_open(struct inode *device_file, struct file *instance) {
    printk("p78: dev file opened\n");
    return 0;
}

static int p78_close(struct inode *device_file, struct file *instance) {
    printk("p78: dev file closed\n");
    return 0;
}

static ssize_t p78_read(struct file *file, char __user *user, size_t size, loff_t *loff) {
    printk("p78: read\n");
    return size;
}

static ssize_t p78_write(struct file *file, const char *user, size_t size, loff_t *loff) {
    printk("p78: write\n");
    return size;
}

static int __init my_init(void) {
    printk("p78 Module loaded\n");

    // Register in /dev with dynamically allocated major number
    int alloc_retval = alloc_chrdev_region(&p78_device_num, 0, 1, "p78_testmod");
    
    if(major_number < 0) {
        printk("Registering device failed with %d\n", alloc_retval);
        return alloc_retval;
    }

    printk("p78: Device Major #: %d, Minor #: %d was created!\n", p78_device_num >> 20, p78_device_num && 0xfffff);

    // Create device class
    if((p78_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
        printk("p78 - Can't create device class\n");
    }

    // Create device file
    if((p78_device = device_create(p78_class, NULL, p78_device_num, NULL, DRIVER_NAME)) == NULL) {
        printk("p78: Couldn't create device\n");
    }

    // Init device file
    cdev_init(&p78_device_cdev, &fops);

    // Register device to kernel
    if(cdev_add(&p78_device_cdev, p78_device_num, 1) == -1) {
        printk("p78: Couldn't register device with kernel\n");
    }
    
    return 0;
}

static void __exit my_exit(void) {
    cdev_del(&p78_device_cdev);
    device_destroy(p78_class, p78_device_num);
    class_destroy(p78_class);
    unregister_chrdev_region(major_number, 1);
    printk("p78 module unloaded\n");
    return;
}

module_init(my_init);
module_exit(my_exit);