/*
 * @Author       : lv zhipeng
 * @Date         : 2022-06-29 16:49:05
 * @LastEditors  : lv zhipeng
 * @LastEditTime : 2022-06-30 09:18:13
 * @FilePath     : /Linux-driver-development-basics-code/5_KEY_Interrupt/5_1_gpio_interrupt_simple/gpio_key_drv.c
 * @Description  :
 *
 */
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <linux/platform_device.h>
#include <asm/io.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>

static int chip_demo_gpio_probe(struct platform_device *pdev)
{
    return 0;
}

static int chip_demo_gpio_remove(struct platform_device *pdev)
{
    return 0;
}

/* 1. 定义platform_driver */
static const struct of_device_id mdxz_gpios[] = {
    {.compatible = "mdxz,leddrv"},
};

static struct platform_driver gpio_key_drv = {
    .probe = chip_demo_gpio_probe,
    .remove = chip_demo_gpio_remove,
    .driver = {
        .name = "mdxz_gpios",
        .of_match_table = mdxz_gpios,
    },
};
/* 2. 在入口函数注册platform_driver */
static int __init mdxz_gpio_drv_init(void)
{
    int ret = -1;

    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    ret = platform_driver_register(&gpio_key_drv);
    return 0;
}

static void __exit mdxz_gpio_drv_exit(void)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    platform_driver_unregister(&gpio_key_drv);
}

module_init(mdxz_gpio_drv_init);
module_exit(mdxz_gpio_drv_exit);
MODULE_LICENSE("GPL");
