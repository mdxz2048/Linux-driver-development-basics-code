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

#include "key_operation.h"
#include "key_drv.h"
/*GPIO5_CLK_ENABLE  KEY1*/
static volatile unsigned int *CCM_CCGR1; // 20C_406C
/*GPIO4_CLK_ENABLE  KEY2*/
static volatile unsigned int *CCM_CCGR3; // 20C_4074

/*SET GPIO5 as GPIO*/
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER1; // 229000Ch
/*SET GPIO4 as GPIO*/
static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_NAND_CE1_B; // 20E01B0h

struct GPIOx_t
{
    volatile unsigned int dr;
    volatile unsigned int gdir;
    volatile unsigned int psr;
    volatile unsigned int icr1;
    volatile unsigned int icr2;
    volatile unsigned int imr;
    volatile unsigned int isr;
    volatile unsigned int edge_sel;

};
static struct GPIOx_t *gpio5_key1; // 0x020AC000
static struct GPIOx_t *gpio4_key2; // 0x020A8000

static int imx6ull_gpio_init(int which)
{
    printk("%s %s line %d, \n", __FILE__, __FUNCTION__, __LINE__);
    CCM_CCGR1 = ioremap(0x020C406C, 4);
    CCM_CCGR3 = ioremap(0x020C4074, 4);

    IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER1 = ioremap(0x0229000C, 4);
    IOMUXC_SW_MUX_CTL_PAD_NAND_CE1_B = ioremap(0x020E01B0, 4);

    gpio5_key1 = ioremap(0x020AC000, sizeof(struct GPIOx_t));
    gpio4_key2 = ioremap(0x020A8000, sizeof(struct GPIOx_t));
    // operations code
    switch (which)
    {
    case 0:
        /*0 enable clk*/
        *CCM_CCGR1 |= (3 << 30);
        /*1 set pin as GPIO*/
        *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER1 = 5;
        /*2 set direction*/
        gpio5_key1->dr &= ~(1 << 1);

        break;
    case 1:
        /*0 enable clk*/
        *CCM_CCGR3 |= (3 << 30);
        /*1 set pin as GPIO*/
        *IOMUXC_SW_MUX_CTL_PAD_NAND_CE1_B = 5;
        /*2 set direction*/
        gpio4_key2->dr &= ~(1 << 14);
        break;
    }
    return 0;
}

static int imx6ull_gpio_read(int which)
{
    printk("%s %s line %d, \n", __FILE__, __FUNCTION__, __LINE__);
    switch (which)
    {
    case 0:
        return (gpio5_key1->psr & (1 << 1)) ? 1 : 0;
    case 1:
        return (gpio4_key2->psr & (1 << 1)) ? 1 : 0;
    }
    return 0;
}

static struct key_operations_t mdxz_key_opr =
    {
        .count = 2,
        .init = imx6ull_gpio_init,
        .read = imx6ull_gpio_read,
};

int imx6ull_key_init(void)
{
    register_key_opration(&mdxz_key_opr);
    return 0;
}

void imx6ull_key_exit(void)
{
    unregister_key_opration(&mdxz_key_opr);
}

module_init(imx6ull_key_init);
module_exit(imx6ull_key_exit);
MODULE_LICENSE("GPL");