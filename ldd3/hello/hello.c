#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/stat.h>

static char *name = "Gauri Shankar Mandal"; /*Default values of module parameters*/
static int loops = 10;
static int arr_argc = 0;
static int arr[5] = {0,0,0,0,0};

module_param(loops, int, S_IRUGO);
module_param(name, charp, S_IRUGO);
module_param_array(arr, int, &arr_argc, S_IRUGO);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gaurimandal50@gmail.com");
MODULE_DESCRIPTION("A simple device driver!");
MODULE_VERSION("0.1");

/*to export symbol*/
extern int greet_func(int i) {
    printk(KERN_INFO "HELLO USER!\n");
    return 0;
}

static int __init registration(void) {
    int i = 0;
    printk(KERN_INFO "Hello user, the hello world driver is loaded successfully.\n");
    for(i = 0; i < loops; i++) {
        printk(KERN_INFO "%s\n", name);
    }
    printk(KERN_INFO "The number of arguments in array is %d.\n", arr_argc);
    return 0;
}

static void __exit deregistration(void) {
    printk(KERN_INFO "Bye bye user, the hello world driver is removed successfully.\n");
}

EXPORT_SYMBOL(greet_func);
module_init(registration);
module_exit(deregistration);
