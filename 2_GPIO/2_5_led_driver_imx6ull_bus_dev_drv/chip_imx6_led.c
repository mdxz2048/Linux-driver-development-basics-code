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

#include "led_operation.h"
#include "led_resource.h"
#include "led_drv.h"

led_operations_t led_opr_100ask;
static int g_ledpins[100];
static int g_ledcnt = 0;

/**
 * @brief Construct a new led opr init object
 *
 * @param which
 */
static int chip_imx6_led_init(int which)
{
    printk("%s %s line %d, \n", __FILE__, __FUNCTION__, __LINE__);
    // operations code
    switch (GROUP(g_ledpins[which]))
    {
    case 0:
        printk("init pin of group 0 ...\n");
        break;
    case 1:
        printk("init pin of group 1 ...\n");
        break;
    case 2:
        printk("init pin of group 2 ...\n");
        break;
    case 3:
        printk("init pin of group 3 ...\n");
        break;

        return 0;
    }
}

static int chip_imx6_led_exit(int which)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    return 0;
}

/**
 * @brief               控制LED
 *
 * @param which         which-哪个LED
 * @param status        status:1-亮,0-灭
 * @return int
 */
static int chip_imx6_led_ctl(int which, int status)
{
    printk("%s %s line %d, %s\n", __FILE__, __FUNCTION__, __LINE__, status ? "on" : "off");
    // operations code
    switch (GROUP(g_ledpins[which]))
    {
    case 0:
        printk("set pin of group 0 ...\n");
        break;
    case 1:
        printk("set pin of group 1 ...\n");
        break;
    case 2:
        printk("set pin of group 2 ...\n");
        break;
    case 3:
        printk("set pin of group 3 ...\n");
        break;
    default:
        break;
    }

    return 0;
}

led_operations_t *get_board_led_opr(void)
{
    led_opr_100ask.init = chip_imx6_led_init;
    led_opr_100ask.ctl = chip_imx6_led_ctl;
    led_opr_100ask.exit = chip_imx6_led_exit;

    return &led_opr_100ask;
}

static int chip_imx6_gpio_probe(struct platform_device *pdev)
{
    for (g_ledcnt = 0; g_ledcnt < pdev->num_resources; g_ledcnt++)
    {
        g_ledpins[g_ledcnt] = pdev->resource[g_ledcnt].start;
    }

    return 0;
}

static int chip_imx_gpio_remove(struct platform_device *pdev)
{
    struct resource *res;
    int i = 0;

    while (1)
    {
        res = platform_get_resource(pdev, IORESOURCE_IRQ, i);
        if (!res)
            break;

        led_destroy_device(i);
        i++;
        g_ledcnt--;
    }
    return 0;
}

static struct platform_driver chip_demo_gpio_driver = {
    .probe = chip_imx6_gpio_probe,
    .remove = chip_imx_gpio_remove,
                  .driver = {
        .name = "mdxz_led",
    },
};


static int __init chip_demo_gpio_drv_init(void)
{
    int err;

    err = platform_driver_register(&chip_demo_gpio_driver);
    register_led_operations(&led_opr_100ask);

    return 0;
}

static void __exit chip_demo_gpio_drv_exit(void)
{
    platform_driver_unregister(&chip_demo_gpio_driver);
}

module_init(chip_demo_gpio_drv_init);
module_exit(chip_demo_gpio_drv_exit);
MODULE_LICENSE("GPL");
