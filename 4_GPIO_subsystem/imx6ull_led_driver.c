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

// static int g_ledpins[100];
// static int g_ledcnt = 0;
#define DEVNAME "mdxz_led"

static int led_major = 0;
static struct class *led_class;
static struct gpio_desc *led_gpio;

static int led_drv_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

/* write(fd, &val, 1); */
static int led_drv_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos)
{
    char value = 0;
    int ret = -1;

    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    /* 拷贝用户层数据 */
    ret = copy_from_user(&value, buf, 1);
    /* 根据次设备号和status控制LED */
    gpiod_set_value(led_gpio, value);

    return 1;
}

static int led_drv_open(struct inode *inode, struct file *filp)
{
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    /* 根据次设备号初始化LED */
    gpiod_direction_output(led_gpio, 0);

    return 0;
}

static int led_drv_close(struct inode *node, struct file *file)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    /*提取次设备号*/

    return 0;
}

static struct file_operations led_drv = {
    .owner = THIS_MODULE,
    .open = led_drv_open,
    .read = led_drv_read,
    .write = led_drv_write,
    .release = led_drv_close,
};

static int chip_demo_gpio_probe(struct platform_device *pdev)
{
    /*1 设备树中定义�? led-gpios=<...>;	*/
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    led_gpio = gpiod_get(&pdev->dev, "led", 0);
    if (IS_ERR(led_gpio))
    {
        dev_err(&pdev->dev, "Failed to get GPIO for led\n");
        return PTR_ERR(led_gpio);
    }
    /*2 注册file_operations 	*/
    led_major = register_chrdev(0, "maxz_led", &led_drv); // /dev/DEVNAME
    if (led_major < 0)
    {
        printk(DEVNAME ": could not get major number\n");
        return led_major;
    }
    /*class_create*/
    led_class = class_create(THIS_MODULE, "mdxz_led_class");
    if (IS_ERR(led_class))
    {
        class_destroy(led_class);
        gpiod_put(led_gpio);
        unregister_chrdev(led_major, DEVNAME);
        return PTR_ERR(led_class);
    }
	device_create(led_class, NULL, MKDEV(led_major, 0), NULL, "mdxz_led%d", 0); /* /dev/100ask_led0 */

    return 0;
}

static int chip_demo_gpio_remove(struct platform_device *pdev)
{
    device_destroy(led_class, MKDEV(led_major, 0));
    class_destroy(led_class);
    unregister_chrdev(led_major, DEVNAME);
    gpiod_put(led_gpio);

    return 0;
}

static const struct of_device_id mdxz_leds[] = {
    {.compatible = "mdxz,leddrv"},
    {},
};

static struct platform_driver chip_demo_gpio_drv = {
    .probe = chip_demo_gpio_probe,
    .remove = chip_demo_gpio_remove,
    .driver = {
        .name = "mdxz_led",
        .of_match_table = mdxz_leds,
    },
};

static int __init chip_demo_gpio_drv_init(void)
{
    int err = -1;
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    err = platform_driver_register(&chip_demo_gpio_drv);
    return 0;
}

static void __exit chip_demo_gpio_drv_exit(void)
{
    platform_driver_unregister(&chip_demo_gpio_drv);
}

module_init(chip_demo_gpio_drv_init);
module_exit(chip_demo_gpio_drv_exit);
MODULE_LICENSE("GPL");