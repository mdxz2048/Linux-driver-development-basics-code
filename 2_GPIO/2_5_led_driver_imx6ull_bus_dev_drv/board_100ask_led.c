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
#include "led_operation.h"
#include <asm/io.h>

static struct platform_device pdev;
static struct resource *led_resources[] = {
    {
        .star = GROUP_PIN(3, 1),
        .name = "mdxz_led",
        .flags = IORESOURCE_IRQ,
    },

};
static int board_imx6ull_led_init();
static int board_imx6ull_led_release();
static int board_imx6ull_led_exit();
MODULE_LICENSE("GPL");

static int board_imx6ull_led_init()
{
    printk("%s %s line %d, %s\n", __FILE__, __FUNCTION__, __LINE__, status ? "on" : "off");

    pdev.name = "mdxz_led";
    pdev.num_resources = ARRAY_SIZE(led_resources);
    pdev.resource = led_resources;
    pdev.dev = {
        .release = board_imx6ull_led_release,
    };

    int ret = platform_device_register(&board_A_led_dev);

    return 0;
}

module_init(board_imx6ull_led_init);

static int board_imx6ull_led_exit()
{
    printk("%s %s line %d, %s\n", __FILE__, __FUNCTION__, __LINE__, status ? "on" : "off");

    int ret = platform_device_unregister(&board_A_led_dev);

    return 0;
}

module_exit(board_imx6ull_led_exit);

static int board_imx6ull_led_release()
{
    return 0;
}
