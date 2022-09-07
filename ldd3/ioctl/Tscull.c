#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/stat.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/ioctl.h>
#include "Tscull.h"

/*https://gist.github.com/itrobotics/063aea3c72d1f7d7fb94*/
/*
Note that ioctl has been removed from file_operations long ago. The latest substitutes are:-
long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
*/
static int major = 0;
static int minor_max = 2;
static int first_minor = 0;
dev_t dev= {0};
char device[] = "scull";
struct cdev my_cdev;
static int     scull_open(struct inode *inod, struct file *fil);
static int     scull_release(struct inode *inod, struct file *fil);
static long    scull_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

char **database = NULL;
char test[11] = {0};

struct file_operations fops = {
    .unlocked_ioctl = scull_unlocked_ioctl,
    .compat_ioctl   = NULL,
    .release        = scull_release,
    .open           = scull_open,
    .owner          = THIS_MODULE,
};

module_param(major, int, S_IRUGO);
module_param(minor_max, int, S_IRUGO);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gaurimandal50@gmail.com");
MODULE_DESCRIPTION("A simple device driver!");
MODULE_VERSION("0.1");
/*
 *Creating ioctl numbers which contain (type = magic number = systemwide unique number for device e.g. major,
 *ordinal no = unique number among all the ioctl commands in same device e.g. 0,1,2,etc.
 *direction bit = W,R,WR markings in macros like _IOW, _IOWR, _IOR, _IO take care of 3rd number
 *size = type of data we want to get/set with the ioctl call.
 *These numbers can be manipulated using various macros provided in ioctl.h. The bit field architecture in
 *ioctl_number is given by various macros in ioctl.h viz _IOC_TYPEBITS (type bits), _IOC_NRBITS (ordinal number bits),
 *_IOC_READ, _IOC_WRITE, and _IOC_READ | _IOC_WRITE (direction bits)
 */


/*ioctl number creating macros. can be used any type of data or pointer in size arg*/
/*
#define SCULL_SET_PARAM _IOW(major, 0, int *)
#define SCULL_GET_PARAM _IOR(major, 1, int*)
#define SCULL_SETGET_PARAM _IOWR(major, 2, int *)
*/


int ioctl_data = 0;
long scull_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    int temp = 0;
    printk(KERN_INFO "Tarzan called scull_unlocked_ioctl method.\n");
    /*_IOC_TYPE, _IOC_NR etc are manipulator macros for ioctl numbers to get particular number viz magic, ordinal, direction, size
     * from ioctl number for verification purpose*/
    if(_IOC_TYPE(cmd) != major) {
        printk(KERN_ERR "Invalid type number found in ioctl number. Invalid ioctl number or command.\n");
        return -ENOTTY;
    }

    if(_IOC_NR(cmd) > 2 /*max value of ordinal numbers (second argument of _IOWR like macros) used in creating ioctl number*/) {
        printk(KERN_ERR "Invalid ordinal number found in ioctl number. Invalid ioctl number or command.\n");
        return -ENOTTY;
    }
    //switch(cmd) {
        //case SCULL_SET_PARAM:
        if(SCULL_SET_PARAM == cmd) {
            copy_from_user(&ioctl_data, (int*)arg, sizeof(int));
            //when arg is passed by value in this case, we do not need to use get_user or copy_from_user 
            printk(KERN_INFO "Set of ioctl_data with value %d succeeded.\n", ioctl_data);
            //break;
        }
        //case SCULL_GET_PARAM:
        else if(SCULL_GET_PARAM == cmd) {
            //put_user(ioctl_data, (int*)arg);
            copy_to_user((int*)arg, &ioctl_data, sizeof(int));
            printk(KERN_INFO "Get of ioctl_data with value %d succeeded.\n", ioctl_data);
            //break;
        }
        //case SCULL_SETGET_PARAM:
        else if(SCULL_SETGET_PARAM == cmd) {
            copy_from_user(&ioctl_data, (int*)arg, sizeof(int));
            printk(KERN_INFO "Set of ioctl_data with value %d succeeded.\n", ioctl_data);
            temp = ioctl_data + 10;
            copy_to_user((int*)arg, &temp, sizeof(int));
            printk(KERN_INFO "Get of ioctl_data plus 10 value %d succeeded.\n", temp);
            //break;
        }
        //case default:
        else {
            printk(KERN_ERR "Invalid ioctl command requested.\n");
            return -ENOTTY;
        }
    //}
    return 0;
}

static int __init registration(void) {
    int ret = 0;
    printk(KERN_INFO "Hello user, the scull driver is loaded successfully.\n");
    if(major) { /*major number provided by user, statically allocating*/
        dev = MKDEV(major, first_minor);
        if(register_chrdev_region(dev, minor_max, device) < 0) {
            printk(KERN_ERR "Error registering device with major no %d.\n", major);
            return -1;
        }
    } else { /*major number not povided, dynamically acllocating*/
        if(alloc_chrdev_region(&dev, first_minor, minor_max, device) < 0 ) {
            printk(KERN_ERR "Error allocating device dynamically.\n");
            return -1;
        }
        major = MAJOR(dev);
        printk(KERN_INFO "Allocated device with major no %d.\n", major);
    }

    /*Method 1 of cdev allocation. RUNTIME ALLOCATION
    
    struct cdev *my_cdev = NULL;
    cdev = cdev_alloc();
    cdev->f_ops = &fops;
    
    */
    /*Method 2 of cdev allocation. OWN ALLOCATION */
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;
    ret = cdev_add(&my_cdev, dev, minor_max);
    if(ret) {
        printk(KERN_ERR "Error adding character device to kernel.\n");
	return -1;
    }

    printk(KERN_INFO "Char device scull has been added to kernel successfully\n");
    return 0;
}

static void __exit deregistration(void) {
    unregister_chrdev_region(dev, minor_max);
    printk(KERN_INFO "Char device scull has been unregistered.\n");
    cdev_del(&my_cdev);
    printk(KERN_INFO "Char device scull has been removed from kernel.\n");
    printk(KERN_INFO "Bye bye user, the driver is removed.\n");
}

module_init(registration);
module_exit(deregistration);

static int scull_open(struct inode *inod, struct file *fil) {
    int major = 0;
    int minor = 0;
    major = imajor(inod);
    minor = iminor(inod);
    printk(KERN_INFO "Someone is opening this module at major %d, minor %d.\n", major, minor);
    major = imajor(fil->f_path.dentry->d_inode);
    minor = iminor(fil->f_path.dentry->d_inode);
    //*(int*)0 = 5;//deliberate bug to produce OOPS
    printk(KERN_INFO "Someone is opening this module at major %d, minor %d.\n", major, minor);
    return 0;
}

static int scull_release(struct inode *inod, struct file *fil) {
    int major = 0;
    int minor = 0;
    major = imajor(inod);
    minor = iminor(inod);
    printk(KERN_INFO "Someone is closing this module at major %d, minor %d.\n", major, minor);
    major = imajor(fil->f_path.dentry->d_inode);
    minor = iminor(fil->f_path.dentry->d_inode);
    printk(KERN_INFO "Someone is closing this module at major %d, minor %d.\n", major, minor);
    return 0;
}
