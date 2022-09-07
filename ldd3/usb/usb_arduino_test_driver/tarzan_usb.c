/*
 * USB LED driver - 1.1
 *
 * Copyright (C) 2004 Greg Kroah-Hartman (greg@kroah.com)
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, version 2.
 *
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL"); //avoid kernel taint warning
#define DRIVER_AUTHOR "Greg Kroah-Hartman, greg@kroah.com"
#define DRIVER_DESC "USB LED Driver"

//#define VENDOR_ID	0x0fc5
//#define PRODUCT_ID	0x1223
//SanDisk cruzer blade

//#define VENDOR_ID  0x0781
//#define PRODUCT_ID 0x5567


//#define VENDOR_ID  0x10c4
//#define PRODUCT_ID 0x8108

#define VENDOR_ID  0x2341
#define PRODUCT_ID 0x0043
/* table of devices that work with this driver */
static struct usb_device_id id_table [] = {
	{ USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
	{ },
};
MODULE_DEVICE_TABLE (usb, id_table);

struct usb_led {
	struct usb_device *	udev;
	unsigned char		blue;
	unsigned char		red;
	unsigned char		green;
};

#define BLUE	0x04
#define RED	0x02
#define GREEN	0x01
static void change_color(struct usb_led *led)
{
	int retval;
	unsigned char color = 0x07;
	unsigned char *buffer;

	buffer = kmalloc(8, GFP_KERNEL);
	if (!buffer) {
		dev_err(&led->udev->dev, "out of memory\n");
		return;
	}

	if (led->blue)
		color &= ~(BLUE);
	if (led->red)
		color &= ~(RED);
	if (led->green)
		color &= ~(GREEN);
	dev_dbg(&led->udev->dev,
		"blue = %d, red = %d, green = %d, color = %.2x\n",
		led->blue, led->red, led->green, color);

	retval = usb_control_msg(led->udev,
				usb_sndctrlpipe(led->udev, 0),
				0x12,
				0xc8,
				(0x02 * 0x100) + 0x0a,
				(0x00 * 0x100) + color,
				buffer,	
				8,
				2000);
	if (retval)
		dev_dbg(&led->udev->dev, "retval = %d\n", retval);
	kfree(buffer);
}

#define show_set(value)	\
static ssize_t show_##value(struct device *dev, struct device_attribute *attr, char *buf)		\
{									\
	struct usb_interface *intf = to_usb_interface(dev);		\
	struct usb_led *led = usb_get_intfdata(intf);			\
									\
	return sprintf(buf, "%d\n", led->value);			\
}									\
static ssize_t set_##value(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)	\
{									\
	struct usb_interface *intf = to_usb_interface(dev);		\
	struct usb_led *led = usb_get_intfdata(intf);			\
	int temp = simple_strtoul(buf, NULL, 10);			\
									\
	led->value = temp;						\
	change_color(led);						\
	return count;							\
}									\
static DEVICE_ATTR(value, S_IRUGO | S_IWUSR, show_##value, set_##value);
show_set(blue);
show_set(red);
show_set(green);

static int led_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	struct usb_device *udev = interface_to_usbdev(interface);
	struct usb_led *dev = NULL;
	int retval = -ENOMEM;

    printk(KERN_ERR "TARZAN ARDUINO UNO DEVICE PLUGGED IN.\n");

	dev = kzalloc(sizeof(struct usb_led), GFP_KERNEL);
	if (dev == NULL) {
		dev_err(&interface->dev, "Out of memory\n");
		goto error_mem;
	}

	dev->udev = usb_get_dev(udev);

	usb_set_intfdata (interface, dev);

    printk(KERN_ERR "TARZAN 1.\n");
	retval = device_create_file(&interface->dev, &dev_attr_blue);
	if (retval)
		goto error;
	retval = device_create_file(&interface->dev, &dev_attr_red);
	if (retval)
		goto error;
	retval = device_create_file(&interface->dev, &dev_attr_green);
	if (retval)
		goto error;

    printk(KERN_ERR "TARZAN 2.\n");
	dev_info(&interface->dev, "USB LED device now attached\n");
	return 0;

error:
	device_remove_file(&interface->dev, &dev_attr_blue);
	device_remove_file(&interface->dev, &dev_attr_red);
	device_remove_file(&interface->dev, &dev_attr_green);
	usb_set_intfdata (interface, NULL);
	usb_put_dev(dev->udev);
	kfree(dev);
error_mem:
    printk(KERN_ERR "TARZAN 3.\n");
	return retval;
}

static void led_disconnect(struct usb_interface *interface)
{
	struct usb_led *dev;

    printk(KERN_ERR "TARZAN ARDUINO UNO DEVICE WAS REMOVED.\n");


	dev = usb_get_intfdata (interface);

    printk(KERN_ERR "TARZAN 100.\n");
	device_remove_file(&interface->dev, &dev_attr_blue);
	device_remove_file(&interface->dev, &dev_attr_red);
	device_remove_file(&interface->dev, &dev_attr_green);

    printk(KERN_ERR "TARZAN 200.\n");
	/* first remove the files, then set the pointer to NULL */
	usb_set_intfdata (interface, NULL);

	usb_put_dev(dev->udev);

	kfree(dev);

    printk(KERN_ERR "TARZAN 300.\n");
	dev_info(&interface->dev, "USB LED now disconnected\n");
}

static struct usb_driver led_driver = {
	.name =		"tarzan_usb",
	.probe =	led_probe,
	.disconnect =	led_disconnect,
	.id_table =	id_table,
};

static int __init usb_led_init(void)
{
	int retval = 0;

	retval = usb_register(&led_driver);
	if (retval)
		printk(KERN_ERR "usb_register failed. Error number %d", retval);
    else
		printk(KERN_ERR "TARZAN usb_register successful\n");

	return retval;
}

static void __exit usb_led_exit(void)
{
	usb_deregister(&led_driver);
	printk(KERN_ERR "TARZAN usb_deregister called\n");
}

module_init (usb_led_init);
module_exit (usb_led_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

