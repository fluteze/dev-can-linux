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
	{ 0x9d9e205d, "gpiod_get_value_cansleep" },
	{ 0xc03196ae, "regulator_enable" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x49cd25ed, "alloc_workqueue" },
	{ 0x22bb1e46, "regmap_write" },
	{ 0x733b4e83, "can_free_echo_skb" },
	{ 0xae324783, "devm_kmalloc" },
	{ 0xcd131960, "can_rx_offload_del" },
	{ 0x52c5c991, "__kmalloc_noprof" },
	{ 0xc81c5d99, "can_put_echo_skb" },
	{ 0xcc232d7a, "regmap_get_val_bytes" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x54953e35, "alloc_canfd_skb" },
	{ 0x4c73ebe2, "hrtimer_init" },
	{ 0x4829a47e, "memcpy" },
	{ 0x867b894d, "spi_write_then_read" },
	{ 0x37a0cba, "kfree" },
	{ 0xec368ebf, "can_skb_get_frame_len" },
	{ 0xc3055d20, "usleep_range_state" },
	{ 0x709c70f0, "netdev_warn" },
	{ 0x43192a78, "devm_gpiod_get_optional" },
	{ 0xf6ebc03b, "net_ratelimit" },
	{ 0xa3a7d96a, "can_rx_offload_threaded_irq_finish" },
	{ 0x6a2a76c0, "open_candev" },
	{ 0x6b09a0bf, "devm_gpiod_put" },
	{ 0x88f990a2, "netdev_err" },
	{ 0xe4ca7d8d, "devm_regulator_get_optional" },
	{ 0x630e032, "__pm_runtime_set_status" },
	{ 0x7c9a7371, "clk_prepare" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xb2fcb56d, "queue_delayed_work_on" },
	{ 0xff2220a7, "pm_runtime_enable" },
	{ 0xe0941e7e, "can_rx_offload_queue_timestamp" },
	{ 0x18c11e82, "alloc_can_skb" },
	{ 0xdbc4e279, "unregister_candev" },
	{ 0x7d9e316d, "can_ethtool_op_get_ts_info_hwts" },
	{ 0x90859bfa, "device_property_read_u32_array" },
	{ 0xc1ab6e84, "can_change_state" },
	{ 0xbc3f2cb0, "timecounter_cyc2time" },
	{ 0xa294bedf, "can_change_mtu" },
	{ 0xc2c5802, "work_busy" },
	{ 0xcfa586b1, "can_rx_offload_add_manual" },
	{ 0x82be2cd4, "spi_sync" },
	{ 0x6f9e763b, "timecounter_read" },
	{ 0xaf013235, "can_dropped_invalid_skb" },
	{ 0x5cba38db, "devm_kfree" },
	{ 0xef05f1f2, "_dev_err" },
	{ 0x3b071a7e, "free_candev" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x2a8eb36f, "sk_skb_reason_drop" },
	{ 0xee8ee71a, "alloc_candev_mqs" },
	{ 0x6047ede6, "can_fd_len2dlc" },
	{ 0xc3063a24, "netdev_printk" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0xe43a779, "spi_get_device_match_data" },
	{ 0x5339880, "driver_unregister" },
	{ 0xf12d9387, "can_fd_dlc2len" },
	{ 0x71630a53, "can_eth_ioctl_hwts" },
	{ 0x1f7155cb, "dql_completed" },
	{ 0x682cb322, "dev_coredumpv" },
	{ 0x2e7563c2, "netif_tx_wake_queue" },
	{ 0x3e65364b, "spi_async" },
	{ 0x8679379e, "close_candev" },
	{ 0xdcb764ad, "memset" },
	{ 0xdf1fff91, "hrtimer_start_range_ns" },
	{ 0x2c4f2e35, "__vmalloc_noprof" },
	{ 0xf47b7dc2, "netif_schedule_queue" },
	{ 0x76174941, "can_bus_off" },
	{ 0x17bd6500, "can_rx_offload_get_echo_skb_queue_timestamp" },
	{ 0x6be0b072, "can_rx_offload_enable" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xc82d4fd3, "devm_clk_get_optional" },
	{ 0x556e4390, "clk_get_rate" },
	{ 0x30bbad06, "spi_setup" },
	{ 0xa4c535e6, "regmap_read" },
	{ 0xdb8cb023, "__pm_runtime_resume" },
	{ 0x9fa7184a, "cancel_delayed_work_sync" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xdf62768, "__spi_register_driver" },
	{ 0x63d7799d, "__kmalloc_cache_noprof" },
	{ 0xb43f9365, "ktime_get" },
	{ 0xf9f8186b, "regmap_bulk_read" },
	{ 0x56470118, "__warn_printk" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0x373b0d5, "dql_reset" },
	{ 0xb6e6d99d, "clk_disable" },
	{ 0xe09d5e1e, "hrtimer_cancel" },
	{ 0xa65c6def, "alt_cb_patch_nops" },
	{ 0xa314ca1a, "regmap_update_bits_base" },
	{ 0x40dcdd15, "regulator_disable" },
	{ 0xee675281, "dev_err_probe" },
	{ 0x1ae9d70b, "alloc_can_err_skb" },
	{ 0x5af46551, "regmap_raw_write" },
	{ 0xfc59ca16, "napi_disable" },
	{ 0x5cf09442, "netdev_notice" },
	{ 0xb40ef982, "__pm_runtime_disable" },
	{ 0x815588a6, "clk_enable" },
	{ 0x27a070e8, "register_candev" },
	{ 0x4cd2998a, "__pm_runtime_idle" },
	{ 0x862258db, "timecounter_init" },
	{ 0xc4f0da12, "ktime_get_with_offset" },
	{ 0x9ff16853, "kmalloc_caches" },
	{ 0x8e36229a, "netdev_info" },
	{ 0x2d3385d3, "system_wq" },
	{ 0x4c59e15d, "__devm_regmap_init" },
	{ 0xb077e70a, "clk_unprepare" },
	{ 0xf7e1b45d, "module_layout" },
};

MODULE_INFO(depends, "can-dev");

MODULE_ALIAS("spi:mcp2517fd");
MODULE_ALIAS("spi:mcp2518fd");
MODULE_ALIAS("spi:mcp251863");
MODULE_ALIAS("spi:mcp251xfd");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp2517fd");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp2517fdC*");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp2518fd");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp2518fdC*");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp251863");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp251863C*");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp251xfd");
MODULE_ALIAS("of:N*T*Cmicrochip,mcp251xfdC*");

MODULE_INFO(srcversion, "EA2A3CE0681AFC1923E2D8C");
