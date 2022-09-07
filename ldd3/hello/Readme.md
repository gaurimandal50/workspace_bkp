sudo apt-get install build-essential linux-headers-$(uname -r)

obj-m = hello.o
all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

/lib/modules/5.4.0-91-generic/build/scripts/sign-file.c  #This signs the loadable module that is not the
	#part of kernel module tree. Otherwise the kernel will not accept any module that is not signed
	#by vendor.
#Error when module not signed and compiled and insmoded:
[26299.407356] hello: loading out-of-tree module taints kernel.
[26299.407437] hello: module verification failed: signature and/or required key missing - tainting kernel

#You are given signing file (c file and binary) to sign your module. File is in build directory ./scripts/sign-file.c
# ./scripts/sign-file.c



sudo insmod hello.ko
sudo rmmod hello

move .ko file to /lib/modules/`uname -r`/
modeprobe hello #NOTE do not use hello.ko with modprobe
modeprobe -r hello

#if modeprobe fails even if .ko file is in /lib/modules/`uname -r`/ then run "sudo depmod" to recreate module dependncy list

