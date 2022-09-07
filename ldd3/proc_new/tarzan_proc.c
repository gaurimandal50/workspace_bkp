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

#define BUFSIZE  100
 
//MODULE_LICENSE("Dual BSD/GPL");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Liran B.H");
 
static struct proc_dir_entry *ent;
 
static ssize_t mywrite(struct file *file, const char __user *ubuf,size_t count, loff_t *ppos) 
{
	printk( KERN_INFO "write handler\n");
	return -1;
}
 
static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{
	static int i = 21;
    if(!access_ok(ubuf, count)) {
        printk(KERN_ERR "Invaild user buffer.\n");
        return -1;
    }
	copy_to_user(ubuf, "Gauri Shankar\n", 10);
	printk( KERN_INFO "read handler\n");
	//return (i-=3);
	return 0; /*This return value is returned as return value of read system call in  user process*/
}
 
static struct file_operations myops = 
{
	.owner = THIS_MODULE,
	.read = myread,
	.write = mywrite,
};
 
static int simple_init(void)
{
	ent=proc_create("tarzanproc",0660,NULL,&myops);
	return 0;
}
 
static void simple_cleanup(void)
{
	proc_remove(ent);
}
 
module_init(simple_init);
module_exit(simple_cleanup);
