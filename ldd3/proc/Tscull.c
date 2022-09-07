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
#include <linux/proc_fs.h>

/*https://gist.github.com/itrobotics/063aea3c72d1f7d7fb94*/
static int major = 0;
static int minor_max = 2;
static int first_minor = 0;
dev_t dev= {0};
char device[] = "scull";
struct cdev my_cdev;
static ssize_t scull_read(struct file *filp, char *buff, size_t len, loff_t *off);
static ssize_t scull_write(struct file *filp, const char *buff, size_t len, loff_t *off);
static int scull_open(struct inode *inod, struct file *fil);
static int scull_release(struct inode *inod, struct file *fil);
static ssize_t scull_proc_read(struct file *filp, char *buff, size_t len, loff_t *off);
char **database = NULL;
char test[11] = {0};

struct scull_dev {
    struct scull_qset *data; /* Pointer to first quantum set */
    int quantum; /* the current quantum size */
    int qset; /* the current array size */
    unsigned long size; /* amount of data stored here */
    unsigned int access_key; /* used by sculluid and scullpriv */
    struct semaphore sem; /* mutual exclusion semaphore*/
    struct cdev cdev; /* Char device structure*/
};

static struct proc_dir_entry *proc_entry;
struct file_operations proc_fops = {
    .read    = scull_proc_read,
    .write   = NULL,
    .release = NULL,
    .open    = NULL,
    .owner   = THIS_MODULE,
    .llseek  = NULL
};

struct file_operations fops = {
 //   .ioctl   = NULL,
    .read    = scull_read,
    .write   = scull_write,
    .release = scull_release,
    .open    = scull_open,
    .owner   = THIS_MODULE,
    .llseek  = NULL
};

module_param(major, int, S_IRUGO);
module_param(minor_max, int, S_IRUGO);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gaurimandal50@gmail.com");
MODULE_DESCRIPTION("A simple device driver!");
MODULE_VERSION("0.1");

int scull_read_procmem(char *buf, char **start, off_t offset,
 int count, int *eof, void *data)
{
    sprintf(buf, "Gauri Shankar Mandal\n");
    return 0;
}

static ssize_t scull_proc_read(struct file *filp, char *buff, size_t len, loff_t *off) {
    char msg[1024] = {0};
    char *msgdyn = NULL;
    int major = imajor(filp->f_path.dentry->d_inode);
    int minor = iminor(filp->f_path.dentry->d_inode);
    printk(KERN_INFO "FILE OPERATION READ:%d:%d\n", major, minor);
    strncpy(msg, "Gauri Shankar Mandal\n", 22);
    printk(KERN_INFO "msg: %s len: %ld\n", msg, len);
    //memset(msgdyn, 'a', len);
    copy_to_user(buff, msg , len);
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

    database = (char**)kmalloc(sizeof(char*)*minor_max, GFP_KERNEL); /*Do free before exiting module or it will stay in RAM until shutdown*/
    if(!database) {
        printk(KERN_ERR "kmalloc failed in init.\n");
	return -1;
    }
    memset(database, 0, sizeof(char*)*minor_max);
    printk(KERN_INFO "Char device scull has been added to kernel successfully. Data allocated.\n");
//    create_proc_entry("scullmem", 0 /* default mode */, NULL /* parent dir */, scull_read_procmem,
//        NULL /* client data */);        //OLD SYLE API, DEPRECATED IN THIS KERNEL VERSION
//    proc_mkdir("scull", NULL);
    proc_entry = proc_create("scullmem", 0644, NULL, &proc_fops);
    if(!proc_entry){
	printk(KERN_INFO "create_proc_entry failed.\n");
	return -1;
    }
    printk(KERN_INFO "/proc/scullmem created\n");
    return 0;
}

static void __exit deregistration(void) {
    //proc_remove("scull", NULL); //deleting directory
    remove_proc_entry("scullmem", NULL /* parent dir */);
    printk(KERN_INFO "/proc/scullmem removed\n");
    unregister_chrdev_region(dev, minor_max);
    printk(KERN_INFO "Char device scull has been unregistered.\n");
    cdev_del(&my_cdev);
    printk(KERN_INFO "Char device scull has been removed from kernel.\n");
    kfree(database);
    printk(KERN_INFO "Bye bye user, the driver is removed. Data freed.\n");
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
    printk(KERN_INFO "Someone is opening this module at major %d, minor %d.\n", major, minor);
    database[minor] = (char*)kmalloc(10, GFP_KERNEL); /*Allocating 10 bytes for each minor number*/
    if(!database[minor]) {
        printk(KERN_ERR "kmalloc failed in open.\n");
	return -1;
    }
    else
       printk(KERN_INFO "kmalloc succeeded in open.\n");
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
    kfree(database[minor]);
    return 0;
}

static ssize_t scull_read(struct file *filp, char *buff, size_t len, loff_t *off) {
    char msg[11] = {0};
    int major = imajor(filp->f_path.dentry->d_inode);
    int minor = iminor(filp->f_path.dentry->d_inode);
    printk(KERN_INFO "FILE OPERATION READ:%d:%d\n", major, minor);
    strncpy(msg, database[minor], len);
    printk(KERN_INFO "msg: %s len: %ld\n", msg, len);
    copy_to_user(buff, msg, len);
    return 0;
}

static ssize_t scull_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
    char msg[11] = {0};
    int major = imajor(filp->f_path.dentry->d_inode);
    int minor = iminor(filp->f_path.dentry->d_inode);
    printk(KERN_INFO "FILE OPERATION WRITE:%d:%d\n", major, minor);
    copy_from_user(msg , buff, len);
    printk(KERN_INFO "WRITING %s.\n", msg);
    strncpy(database[minor], msg, len);
    return 0;
}
