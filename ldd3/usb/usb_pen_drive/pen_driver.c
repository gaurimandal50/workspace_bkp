#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/usb.h>

#include <linux/kref.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

struct usb_skel {
	struct usb_device	*udev;			/* the usb device for this device */
	struct usb_interface	*interface;		/* the interface for this device */
	struct semaphore	limit_sem;		/* limiting the number of writes in progress */
	struct usb_anchor	submitted;		/* in case we need to retract our submissions */
	struct urb		*bulk_in_urb;		/* the urb to read data with */
	unsigned char           *bulk_in_buffer;	/* the buffer to receive data */
	size_t			bulk_in_size;		/* the size of the receive buffer */
	size_t			bulk_in_filled;		/* number of bytes in the buffer */
	size_t			bulk_in_copied;		/* already copied to user space */
	__u8			bulk_in_endpointAddr;	/* the address of the bulk in endpoint */
	__u8			bulk_out_endpointAddr;	/* the address of the bulk out endpoint */
	int			errors;			/* the last request tanked */
	bool			ongoing_read;		/* a read is going on */
	spinlock_t		err_lock;		/* lock for errors */
	struct kref		kref;
	struct mutex		io_mutex;		/* synchronize I/O with disconnect */
	unsigned long		disconnected:1;
	wait_queue_head_t	bulk_in_wait;		/* to wait for an ongoing read */
};


MODULE_LICENSE("GPL"); //avoid kernel taint warning
MODULE_AUTHOR("gaurimandal50@gmail.com");
MODULE_DESCRIPTION("simple usb pen drive driver probing");

//Arduino UNO
#define VENDOR_ID_1  0x2341
#define PRODUCT_ID_1 0x0043

//Arduino MEGA
#define VENDOR_ID_2  0x2341
#define PRODUCT_ID_2 0x0042

//SanDisk Cruzer Blade
#define VENDOR_ID_3  0x0781
#define PRODUCT_ID_3 0x5567

static char buf[256] = {"GAURI SHANKAR MANDAL"};
static struct usb_device_id usb_id_table[] = {
    { USB_DEVICE(VENDOR_ID_1, PRODUCT_ID_1) },
    { USB_DEVICE(VENDOR_ID_2, PRODUCT_ID_2) },
    {}
};

MODULE_DEVICE_TABLE(usb, usb_id_table);  //what is usb here?
static void skel_read_bulk_callback(struct urb *ub)
{
    printk(KERN_ERR "TARZAN skel_read_bulk_callback called\n");
    printk("TARZAN : DATA = %s\n", buf);
}

static int usb_probe(struct usb_interface *interface, const struct usb_device_id *id) {
    int i = 0;
    int retval = 0;
    struct usb_endpoint_descriptor *endpoint = NULL;
    struct urb *urbblkin = NULL;
    char *buf = buf;
    struct usb_skel *dev = NULL;
    int buffer_size = 0;
    int count = 0;

    printk(KERN_ERR "TARZAN usb_probe called idVendor:idProduct = %X:%X\n", id->idVendor,id->idProduct);
    dev_err(&interface->dev, "TARZAN bNumEndpoints %d\n", interface->cur_altsetting->desc.bNumEndpoints);
    dev_err(&interface->dev, "TARZAN bInterfaceNumber %d\n", interface->cur_altsetting->desc.bInterfaceNumber);
    dev_err(&interface->dev, "TARZAN bLength %d\n", interface->cur_altsetting->desc.bLength);
    for(; i < interface->cur_altsetting->desc.bNumEndpoints; i++) {
        if(((interface->cur_altsetting->endpoint + i)->desc.bEndpointAddress & USB_DIR_IN) \
                && (((interface->cur_altsetting->endpoint + i)->desc.bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) \
                    == USB_ENDPOINT_XFER_BULK)) {
            printk(KERN_ERR "TARZAN FOUND A BUILK IN ENDPOINT AFTER ITERATING FOR %d TIMES\n", i);
        }
/*
        if(!((interface->cur_altsetting->endpoint + i)->desc.bEndpointAddress & USB_DIR_IN) \
                && (((interface->cur_altsetting->endpoint + i)->desc.bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) \
                    == USB_ENDPOINT_XFER_BULK)) {
            printk(KERN_ERR "TARZAN FOUND A BUILK OUT ENDPOINT AFTER ITERATING FOR %d TIMES\n", i);
        }
*/
        /*
         * Note that IN and OUT are named in perspective to host machine and not that to device. This is why
         * the structures are named like usb_host_endpoint, etc.
         */
        endpoint = &(interface->cur_altsetting->endpoint + i)->desc;
    }

    buffer_size = endpoint->wMaxPacketSize;
    dev = kmalloc(sizeof(struct usb_skel), GFP_KERNEL);
    dev->bulk_in_size = buffer_size;
    dev->bulk_in_endpointAddr = endpoint->bEndpointAddress;
    dev->bulk_in_buffer = kmalloc(buffer_size, GFP_KERNEL);
    dev->udev = kmalloc(sizeof(struct usb_device), GFP_KERNEL);
    if (!dev->bulk_in_buffer || !dev->udev) {
        printk(KERN_ERR "Could not allocate bulk_in_buffer");
        goto error;
    }

    /*allocating and initializing struct urb in bulk in configuration */
    urbblkin = usb_alloc_urb(0, GFP_KERNEL);
    if(!urbblkin) {
        printk(KERN_ERR "Could not allocate urb");
        retval = -ENOMEM;
        goto error;
    }

/*    buf = usb_buffer_alloc(udev, 256, GFP_KERNEL, &urb->transfer_dma);
    if (!buf) {
        retval = -ENOMEM;
        goto error;
    }
*/
    printk(KERN_ERR "Initiaizing urb");
    /* initialize the urb properly */
    usb_fill_bulk_urb(urbblkin, dev->udev,
        usb_rcvbulkpipe(dev->udev, dev->bulk_in_endpointAddr),
        dev->bulk_in_buffer, dev->bulk_in_size, skel_read_bulk_callback, dev);
        urbblkin->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
        printk(KERN_ERR "Initiaized urb");


     /*send the data out the bulk port */
    retval = usb_submit_urb(urbblkin, GFP_ATOMIC);
/*    retval = usb_bulk_msg (dev->udev,
                       usb_rcvbulkpipe (dev->udev,
                       dev->bulk_in_endpointAddr),
                       dev->bulk_in_buffer,
                       dev->bulk_in_size,
                       &count, HZ*10);
*/
    if (retval) {
        printk(KERN_ERR "%s - failed submitting write urb, error %d", __FUNCTION__, retval);
        goto error;
    }
        printk(KERN_ERR "Submitted urb and read [%s]", dev->bulk_in_buffer);
    return 0;
error:
    printk(KERN_ERR "-EBUSY %d\n", -EBUSY);
    printk(KERN_ERR "-ENODEV %d\n", -ENODEV);
    printk(KERN_ERR "-ENOENT %d\n", -ENOENT);
    printk(KERN_ERR "-ENXIO %d\n", -ENXIO);
    printk(KERN_ERR "-EINVAL %d\n", -EINVAL);
    printk(KERN_ERR "-EXDEV %d\n", -EXDEV);
    printk(KERN_ERR "-EFBIG %d\n", -EFBIG);
    printk(KERN_ERR "-EPIPE %d\n", -EPIPE);
    printk(KERN_ERR "-EMSGSIZE %d\n", -EMSGSIZE);
    printk(KERN_ERR "-ENOSPC %d\n", -ENOSPC);
    printk(KERN_ERR "-ESHUTDOWN %d\n", -ESHUTDOWN);
    printk(KERN_ERR "-EPERM %d\n", -EPERM);
    printk(KERN_ERR "-EHOSTUNREACH %d\n", -EHOSTUNREACH);
    printk(KERN_ERR "-ENOEXEC %d\n", -ENOEXEC);
    printk(KERN_ERR "TARZAN error occurred in probe function.\n");
    return retval;
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
