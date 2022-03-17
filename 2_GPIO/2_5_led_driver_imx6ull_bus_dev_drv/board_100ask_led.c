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
#include <linux/platform_device.h>
#include "led_resource.h"

static struct resource led_resources[] = {
    {
        .start = GROUP_PIN(3, 1),
        .name = "mdxz_led",
        .flags = IORESOURCE_IRQ,
    },
    {
        .start = GROUP_PIN(4, 1),
        .name = "mdxz_led",
        .flags = IORESOURCE_IRQ,
    },

};

static int board_imx6ull_led_init(void);
static void board_imx6ull_led_release(struct device *dev);
static void board_imx6ull_led_exit(void);
MODULE_LICENSE("GPL");

static struct platform_device pdev = {
    .name = "mdxz_led",
    .num_resources = ARRAY_SIZE(led_resources),
    .resource = led_resources,
    .dev = {
        .release = board_imx6ull_led_release,
    },
};
static int board_imx6ull_led_init(void)
{
    int ret;
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

    ret = platform_device_register(&pdev);

    return 0;
}

module_init(board_imx6ull_led_init);

static void board_imx6ull_led_exit(void)
{
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    platform_device_unregister(&pdev);

}

module_exit(board_imx6ull_led_exit);

static void board_imx6ull_led_release(struct device *dev)
{
}
