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

/*REGISTER ADDRESS */
static volatile uint32_t *SW_MUX_CTL_PAD_SNVS_TAMPER3; // 2290014h
static volatile uint32_t *GPIO5_GDIR;                  // 20AC004h
static volatile uint32_t *GPIO5_DR;                    // 20AC000h
static volatile uint32_t *CCM_CCGR1;                   // 0x2290014

static int led_opr_init(void);
static int led_opr_ctl(int status);
static int led_opr_exit(void);

led_operations_t led_opr_100ask;

/**
 * @brief Construct a new led opr init object
 *
 * @param which
 */
static int led_opr_init(void)
{
    printk("%s %s line %d, \n", __FILE__, __FUNCTION__, __LINE__);
    // operations code
    /*IOREMAP*/
    if (!CCM_CCGR1)
    {
        CCM_CCGR1 = ioremap(0x20C406C, 4);
        SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x2290014, 4);
        GPIO5_GDIR = ioremap(0x20AC004, 4);
        GPIO5_DR = ioremap(0x20AC000, 4);
    }
    /*CONFIG GPIO */
    *SW_MUX_CTL_PAD_SNVS_TAMPER3 &= ~0xf; // clear
    *SW_MUX_CTL_PAD_SNVS_TAMPER3 |= 0x5;  // set
    /*CONFIG GPIO OUTPUT*/
    *GPIO5_GDIR |= (1 << 3);

    return 0;
}

static int led_opr_exit(void)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    // operations code
    /*IOUNMAP*/
    iounmap(CCM_CCGR1);
    iounmap(SW_MUX_CTL_PAD_SNVS_TAMPER3);
    iounmap(GPIO5_GDIR);
    iounmap(GPIO5_DR);

    return 0;
}

/**
 * @brief               控制LED
 *
 * @param which         which-哪个LED
 * @param status        status:1-亮,0-灭
 * @return int
 */
static int led_opr_ctl(int status)
{
    printk("%s %s line %d, %s\n", __FILE__, __FUNCTION__, __LINE__, status ? "on" : "off");
    // operations code
    if (status == 0)
    {
        // set GPIO to LED OFF
        *GPIO5_DR |= (1 << 3); // set 3 bit
    }
    else
    {
        // set GPIO to LED ON
        *GPIO5_DR &= ~(1 << 3); // clear 3 bit
    }

    return 0;
}

led_operations_t *get_board_led_opr(void)
{
    led_opr_100ask.init = led_opr_init;
    led_opr_100ask.ctl  = led_opr_ctl;
    led_opr_100ask.exit = led_opr_exit;

    return &led_opr_100ask;
}