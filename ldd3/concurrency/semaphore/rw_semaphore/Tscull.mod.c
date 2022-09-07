#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xb3753869, "module_layout" },
	{ 0x5ab5b891, "param_ops_int" },
	{ 0x78f44845, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x7afe113a, "cdev_add" },
	{ 0xa3036ef8, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0x7b4da6ff, "__init_rwsem" },
	{ 0x56470118, "__warn_printk" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xdecd0b29, "__stack_chk_fail" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x9166fada, "strncpy" },
	{ 0x53b954a2, "up_read" },
	{ 0xf5cb25c8, "kmem_cache_alloc_trace" },
	{ 0x35216b26, "kmalloc_caches" },
	{ 0x668b19a1, "down_read" },
	{ 0x37a0cba, "kfree" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "B791C15ADD25065B1ED3414");
