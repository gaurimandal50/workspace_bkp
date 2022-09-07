[ 8061.233431] usb 2-2: New USB device found, idVendor=0781, idProduct=5567, bcdDevice= 1.00
SanDisk cruzer blade
--------
[11317.163623] usb 2-3: New USB device found, idVendor=10c4, idProduct=8108, bcdDevice= 0.00
DELL mouse
original external mouse driver name "hid_generic"
--------
[18215.378726] usb 2-2: New USB device found, idVendor=2341, idProduct=0043, bcdDevice= 0.01
Arduino UNO
-----------
HOW I TESTED:
I tried using vendor id and product id of my mouse and sandisk curzer blade pen drive but
my driver function was not called because system already had a device driver to handle
those drivers. I tried unloading them but they automatically got loaded right when I 
plugged the device.

Then I thought of an USB device whose driver might not be already in the system.
I had Arduino UNO and I tried using it's vendor ID and product ID but I found that
there is some cdc_acm driver that was invoked. I couln't understand much I just
unloaded it and then I reinserted Arduino's cable. Suddely UREKA moment was there.
My device driver was called.
However, the cdc_acm was reloaded again. Still I could see my device driver being invoked.
----------
