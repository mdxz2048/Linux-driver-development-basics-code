/*
 * @Author       : lv zhipeng
 * @Date         : 2022-06-29 16:49:05
 * @LastEditors  : mdxz2048
 * @LastEditTime : 2022-07-05 21:17:42
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
#include <linux/gpio/consumer.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>

struct gpio_key
{
    int gpio;
    struct gpio_desc *gpiod;
    int flag;
    int irq;
};

static struct gpio_key *mdxz_gpio_key = NULL;

static irqreturn_t mdxz_gpio_interrupt_handler(int irq, void *dev_id)
{
    struct gpio_key *gpio_key = dev_id;
    int val;
    val = gpiod_get_value(gpio_key->gpiod);

    printk("key %d %d\n", gpio_key->gpio, val);
    return IRQ_HANDLED;
}
/* 1. 从platform_device获得GPIO
 * 2. gpio=>irq
 * 3. request_irq
 */
static int chip_demo_gpio_probe(struct platform_device *pdev)
{
    struct device_node *node = pdev->dev.of_node;
    int gpio_count = 0;
    int i = 0, err;
    enum of_gpio_flags flags;
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    /* 1. 从platform_device获得GPIO */
    /* 2. gpio=>irq */
    /* 3. request_irq */
    gpio_count = of_gpio_count(node);
    if (!gpio_count)
    {
        printk("%s %s line %d, there isn't any gpio available\n", __FILE__, __FUNCTION__, __LINE__);
        return -1;
    }
    mdxz_gpio_key = (struct gpio_key *)kmalloc(sizeof(struct gpio_key) * gpio_count, GFP_KERNEL);

    for (i = 0; i < gpio_count; i++)
    {
        // get flags
        mdxz_gpio_key[i].gpio = of_get_gpio_flags(node, i, &flags);
        if (mdxz_gpio_key[i].gpio < 0)
        {
            printk("%s %s line %d, of_get_gpio_flags fail\n", __FILE__, __FUNCTION__, __LINE__);
            return -1;
        }
        mdxz_gpio_key[i].gpiod = gpio_to_desc(mdxz_gpio_key[i].gpio);
        err = devm_gpio_request_one(&pdev->dev, mdxz_gpio_key[i].gpio, flags, NULL);
        mdxz_gpio_key[i].irq = gpio_to_irq(mdxz_gpio_key[i].gpio);

        for (i = 0; i < gpio_count; i++)
        {
            err = request_irq(mdxz_gpio_key[i].irq, mdxz_gpio_interrupt_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "mdxz_gpios", &mdxz_gpio_key[i]);
        }
    }
    return 0;
}

static int chip_demo_gpio_remove(struct platform_device *pdev)
{
    struct device_node *node = pdev->dev.of_node;
    int gpio_count = 0;
    int i;

    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    gpio_count = of_gpio_count(node);
    for (i = 0; i < gpio_count; i++)
    {
        free_irq(mdxz_gpio_key[i].irq, &mdxz_gpio_key[i]);
    }
    kfree(mdxz_gpio_key);
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
