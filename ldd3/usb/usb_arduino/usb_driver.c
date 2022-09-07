#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL"); //avoid kernel taint warning
MODULE_AUTHOR("gaurimandal50@gmail.com");
MODULE_DESCRIPTION("simple test driver");

//Arduino UNO 
#define VENDOR_ID  0x2341
#define PRODUCT_ID 0x0043
static struct usb_device_id usb_id_table[] = {
    { USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
    {}
};

MODULE_DEVICE_TABLE(usb, usb_id_table);  //what is usb here?

static int usb_probe(struct usb_interface *interface, const struct usb_device_id *id) {
    printk(KERN_ERR "TARZAN usb_probe called.\n");
    return 0;
}

static void usb_disconnect(struct usb_interface *interface) {
    printk(KERN_ERR "TARZAN usb_disconnect called.\n");
    return;
}

static struct usb_driver test_usb_driver = {
    .name       = "usb_driver",
    .probe      = usb_probe,
    .disconnect = usb_disconnect,
    .id_table   = usb_id_table
};

static int usb_driver_init(void) {
    int retval = 0;
    printk(KERN_ERR "TARZAN usb driver initialized.\n");
    retval = usb_register(&test_usb_driver);
    if(retval) {
        printk(KERN_ERR "TARZAN failed to register usb_driver.\n");
        return retval;
    }
    return 0;
}

static void usb_driver_exit(void) {
    usb_deregister(&test_usb_driver);
    printk(KERN_ERR "TARZAN usd driver denitialized.\n");
    return;
}

module_init(usb_driver_init);
module_exit(usb_driver_exit);
