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

static struct led_resource *led_rsc;
/**
 * @brief Construct a new led opr init object
 *
 * @param which
 */
static int chip_imx6_led_init(int which)
{
    printk("%s %s line %d, \n", __FILE__, __FUNCTION__, __LINE__);
    led_rsc = get_led_resouce();
    printk("%s %s line %d, \n", __FILE__, __FUNCTION__, __LINE__);
    // operations code
    printk("init Num of led %d\n", which);
    switch (GROUP(led_rsc->pin))
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
    }
    return 0;
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
    printk("ctrl Num of led %d,The status is %d\n", which, status);
    switch (GROUP(led_rsc->pin))
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

static struct led_operations_t led_opr_100ask =
    {
        .init = chip_imx6_led_init,
        .ctl = chip_imx6_led_ctl,
        .exit = chip_imx6_led_exit,
};
struct led_operations_t *get_board_led_opr(void)
{
    return &led_opr_100ask;
}
