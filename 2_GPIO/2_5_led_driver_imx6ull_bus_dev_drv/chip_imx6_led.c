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
    default:
        break;
    }

    return 0;
}
module_init(board_imx6ull_led_init);

static int chip_imx6_led_exit(int which)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

    return 0;
}
module_exit(board_imx6ull_led_exit);

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
    led_opr_100ask.init = led_opr_init;
    led_opr_100ask.ctl = led_opr_ctl;
    led_opr_100ask.exit = led_opr_exit;

    return &led_opr_100ask;
}