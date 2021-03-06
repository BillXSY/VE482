//
// Created by citrate on 11/23/21.
//

#include <linux/module.h>
#include <linux/printk.h>
//TODO: Add necessary header files
#include "dice.h"

int dice_major;                /* Major number of dice devices */
int dice_devs = DICE_DEVS;     /* Number of dice devices */
int dice_num = DICE_NUM;       /* Initial number of dice in a device */
int gen_sides = GEN_SIDES;     /* Initial number of sides of `arbitrary` dice game */
struct dice_dev *dice_devices; /* An array of dice device */
struct class *dice_class;      /* Used for class */

static int __init dice_init(void);
static void __exit dice_exit(void);
static int dice_open(struct inode *inode, struct file *filp);
static int dice_release(struct inode *inode, struct file *filp);
static ssize_t dice_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t dice_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);

MODULE_LICENSE("GPL");
module_init(dice_init);
module_exit(dice_exit);
//TODO: Make gen_sides a module parameter

static struct file_operations fops = {
        .read = dice_read,
        .write = dice_write,
        .open = dice_open,
        .release = dice_release,
};

static int __init dice_init(void) {
    //TODO: Find Major number dynamically
    // Hint: alloc_chrdev_region
    dev_t dev;
    alloc_chrdev_region(&dev, 0, 3, "dice");
    dice_major = MAJOR(dev);



    //TODO: Allocate memory for dices
    struct class * class_create(THIS_MODULE, "diceClass");



    //TODO: create a class, loop through registering cdev and creating device
    // Hint: class_create, cdev_init, cdev_add, device_create


    

    return 0;
}

static void __exit dice_exit(void) {
    //TODO: release all resources
    // Hint: Pay attention to the order!
    // Hint: device_destroy, cdev_del, class_destroy, kfree, unregister_chrdev_region

    dev_t first = MKDEV(dice_major, 0);
    unregister_chrdev_region(first, 3);

    class_destory()
}

static int dice_open(struct inode *inode, struct file *filp) {
    //TODO: Find which dice is opened
    // Hint: container_of, filp->private_data
    return 1;
}

static int dice_release(struct inode *inode, struct file *filp) {
    //Do nothing here
    //If you allocate any resource in dice_open(), then you must release them here
    return 0;
}

static ssize_t dice_read(struct file *filp, char __user *buff, size_t count, loff_t *offp) {
    //TODO: Generate dice patterns, generate random number
    // Attention: handle count and offp carefully
    // Hint: copy_to_user
    return ssize_t(0);
}

static ssize_t dice_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
    //TODO: Read in number of dice
    // Attention: handle count and offp carefully
    // Hint: copy_from_user
    return ssize_t(0);

}

module_init(dice_init);
module_exit(dice_exit);

MODULE_LICENSE("GPL");
