#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xdf62768, "__spi_register_driver" },
	{ 0x4c66f4c3, "consume_skb" },
	{ 0x733b4e83, "can_free_echo_skb" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x2a8eb36f, "sk_skb_reason_drop" },
	{ 0xaf013235, "can_dropped_invalid_skb" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xa65c6def, "alt_cb_patch_nops" },
	{ 0x9555e77f, "_dev_warn" },
	{ 0xc3063a24, "netdev_printk" },
	{ 0x5339880, "driver_unregister" },
	{ 0xdcb764ad, "memset" },
	{ 0x82be2cd4, "spi_sync" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xef05f1f2, "_dev_err" },
	{ 0x867b894d, "spi_write_then_read" },
	{ 0x18c11e82, "alloc_can_skb" },
	{ 0x35dd280, "netif_rx" },
	{ 0x4829a47e, "memcpy" },
	{ 0x34f9dbcd, "gpiochip_get_data" },
	{ 0xafd3e9ef, "mutex_lock" },
	{ 0xa82cc74e, "mutex_unlock" },
	{ 0xdbc4e279, "unregister_candev" },
	{ 0x40dcdd15, "regulator_disable" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0xb6e6d99d, "clk_disable" },
	{ 0xb077e70a, "clk_unprepare" },
	{ 0x3b071a7e, "free_candev" },
	{ 0xb43f9365, "ktime_get" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0x2e7563c2, "netif_tx_wake_queue" },
	{ 0x76174941, "can_bus_off" },
	{ 0x1ae9d70b, "alloc_can_err_skb" },
	{ 0x3399abe1, "can_get_echo_skb" },
	{ 0x88f990a2, "netdev_err" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xc82d4fd3, "devm_clk_get_optional" },
	{ 0x556e4390, "clk_get_rate" },
	{ 0xee8ee71a, "alloc_candev_mqs" },
	{ 0x7c9a7371, "clk_prepare" },
	{ 0x815588a6, "clk_enable" },
	{ 0xe43a779, "spi_get_device_match_data" },
	{ 0x30bbad06, "spi_setup" },
	{ 0xe4ca7d8d, "devm_regulator_get_optional" },
	{ 0xc03196ae, "regulator_enable" },
	{ 0x49cd25ed, "alloc_workqueue" },
	{ 0x125684ad, "rt_mutex_base_init" },
	{ 0xd2afd73d, "__mutex_rt_init" },
	{ 0xae324783, "devm_kmalloc" },
	{ 0x27a070e8, "register_candev" },
	{ 0xe3a0cac5, "device_property_present" },
	{ 0x8e36229a, "netdev_info" },
	{ 0x63c1cdd7, "devm_gpiochip_add_data_with_key" },
	{ 0x90859bfa, "device_property_read_u32_array" },
	{ 0x8679379e, "close_candev" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x27bbf221, "disable_irq_nosync" },
	{ 0xfcec0987, "enable_irq" },
	{ 0x817e997, "netif_device_attach" },
	{ 0x6a2a76c0, "open_candev" },
	{ 0xfcc54ce3, "__dev_fwnode" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xc81c5d99, "can_put_echo_skb" },
	{ 0xa294bedf, "can_change_mtu" },
	{ 0xcd7bd3a6, "ethtool_op_get_ts_info" },
	{ 0xf7e1b45d, "module_layout" },
};

MODULE_INFO(depends, "can-dev");

MODULE_ALIAS("spi:mcp2510");
MODULE_ALIAS("spi:mcp2515");
MODULE_ALIAS("spi:mcp25625");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp2510");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp2510C*");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp2515");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp2515C*");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp25625");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp25625C*");

MODULE_INFO(srcversion, "F41E96270BEA486C49E5B19");
