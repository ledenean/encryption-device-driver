/**************************************************************
* Class:  CSC-415-01 Fall 2022
* Name: Denean Le
* Student ID: 921330745
* GitHub ID: ledenean
* Project: Assignment 6 – Device Driver
*
* File: Le_Denean_HW6_main.c
*
* Description: Device driver that can be loaded and ran in Linux
**************************************************************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

#include <linux/sched.h>
#define MY_MAJOR 415
#define MY_MINOR 0
#define MAX_SIZE 512
#define DEVICE_NAME "Encryption"

int major, minor;
char *kernelBuffer;
struct cdev myCDev;
int actualRxSize = 0;

MODULE_AUTHOR("Denean Le");
MODULE_DESCRIPTION("A simple encryption program");
MODULE_LICENSE("GPL");


// This write function copies a string from the user and assigns it to the kernel buffer
static ssize_t myWrite(struct file *fs, const char __user * buf, size_t hsize, loff_t * off){
    copy_from_user(kernelBuffer, buf, hsize);
    printk(KERN_INFO "We wrote : %lu\n", hsize);
    return hsize;

}
// This read function copies a string to the user buffer from the kernel buffer
static ssize_t myRead(struct file *fs, char __user * buf, size_t hsize, loff_t * off){
    copy_to_user(buf, kernelBuffer, MAX_SIZE);
    printk(KERN_INFO "We read : %lu\n", MAX_SIZE);
    return MAX_SIZE;

}
// Opens the file
static int myOpen(struct inode *inode, struct file *fs){
    // Allocate space for string, 
    // Returns -1 if unsuccessful, otherwise returns 0
    kernelBuffer = vmalloc(MAX_SIZE);
    if(kernelBuffer == 0){
        printk(KERN_INFO "Can not vmalloc. File not opened.\n");
        return -1;
    }
    return 0;
}
// Closes the file
static int myClose(struct inode * inode, struct file * fs){
    vfree(kernelBuffer);
    return 0;
}

// This ioctl function determines if string will encrypted or decrypted
// Switches based on commands: 0 for encryption, 1 for decryption
static long myIoCtl (struct file *fs, unsigned int command, unsigned long data){
    // Had to declare i before for loops because kernel said so
    int i;

    switch(command){
        case 1: 
            // Encryption based on Caesar Cipher Algorithm: substitutes letters with a fixed \
            number of spaces ahead in the alphabet
            // In this case, I chose five spaces ahead in the alphabet
            for(i = 0; i < MAX_SIZE; i++){
                kernelBuffer[i] = kernelBuffer[i] + 5;
            }
            break;

        case 2: 
            for(i = 0; i < MAX_SIZE; i++){
                kernelBuffer[i] = kernelBuffer[i] - 5;
            }
            break;
    }
    return 0;
}

// another data structure
struct file_operations fops = {
    .open = myOpen,
    .release = myClose,
    .read = myRead,
    .write = myWrite,
    .unlocked_ioctl = myIoCtl,
    .owner = THIS_MODULE,
};
// creates a device node in /dev, returns error if not made
int init_module(void) {
    int result, registers;
    dev_t devno;

    devno = MKDEV(MY_MAJOR, MY_MINOR);

    registers = register_chrdev_region(devno, 1, DEVICE_NAME);
    printk(KERN_INFO "Register chardev succeeded 1: %d\n", registers);
    cdev_init(&myCDev, &fops);
    myCDev.owner = THIS_MODULE;

    result = cdev_add(&myCDev, devno, 1);
    printk(KERN_INFO "Dev Add chardev succeeded 2: %d\n", result);

    if(result < 0){
        printk(KERN_ERR "Register chardev failed : %d\n", result);
    }
    return result;
}

// unregistering and removing device from kernel
void cleanup_module(void){
    dev_t devno;
    devno = MKDEV(MY_MAJOR, MY_MINOR);
    unregister_chrdev_region(devno, 1);
    cdev_del(&myCDev);

    printk(KERN_INFO "Goodbye from Sample NULL Driver!\n");

}