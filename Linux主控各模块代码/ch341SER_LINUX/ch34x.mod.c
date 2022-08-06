#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x98397cc5, "module_layout" },
	{ 0x86b07527, "usb_serial_resume" },
	{ 0xd079479, "usb_serial_suspend" },
	{ 0xce2a3bca, "usb_serial_disconnect" },
	{ 0xae45203f, "usb_serial_probe" },
	{ 0x2bca00e2, "usb_serial_deregister" },
	{ 0x9cbe169c, "usb_deregister" },
	{ 0x6ab994, "usb_register_driver" },
	{ 0x37920b1c, "usb_serial_register" },
	{ 0xca8bf580, "usb_serial_port_softint" },
	{ 0x2e60bace, "memcpy" },
	{ 0xdfe724e7, "tty_flip_buffer_push" },
	{ 0xdda37415, "tty_insert_flip_string_flags" },
	{ 0xd05d3f6a, "tty_buffer_request_room" },
	{ 0x37a0cba, "kfree" },
	{ 0x2559d45b, "dev_set_drvdata" },
	{ 0x48eb0c0d, "__init_waitqueue_head" },
	{ 0x992847d2, "kmalloc_caches" },
	{ 0x156b0a42, "kmem_cache_alloc_trace" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xd99cc8f4, "usb_clear_halt" },
	{ 0xfd41107f, "tty_encode_baud_rate" },
	{ 0x5e774182, "tty_get_baud_rate" },
	{ 0xf2997713, "tty_termios_hw_change" },
	{ 0x67b27ec1, "tty_std_termios" },
	{ 0xca63aac8, "usb_kill_urb" },
	{ 0xd3ae0a10, "usb_control_msg" },
	{ 0xe45f60d8, "__wake_up" },
	{ 0x50eedeb8, "printk" },
	{ 0x231d3c74, "dev_printk" },
	{ 0x1d0c1b88, "dev_err" },
	{ 0xb734a578, "usb_submit_urb" },
	{ 0x75bb675a, "finish_wait" },
	{ 0x622fa02a, "prepare_to_wait" },
	{ 0x4292364c, "schedule" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x5f5602c6, "current_task" },
	{ 0xf97456ea, "_raw_spin_unlock_irqrestore" },
	{ 0x21fb443e, "_raw_spin_lock_irqsave" },
	{ 0xed28e691, "dev_get_drvdata" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=usbserial";

MODULE_ALIAS("usb:v1A86p7523d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1A86p5523d*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "E4C8A47A5DBE4E4A9A41F20");
