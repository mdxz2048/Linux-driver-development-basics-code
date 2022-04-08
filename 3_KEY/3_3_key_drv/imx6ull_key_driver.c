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

static int imx6ull_gpio_init(int which)
{
    printk("%s %s line %d, \n", __FILE__, __FUNCTION__, __LINE__);
    // operations code
    switch (which)
    {
    case 0:
        printk("init key 0 ...\n");
        break;
    case 1:
        printk("init key 1 ...\n");
        break;
    case 2:
        printk("init key 2 ...\n");
        break;
    case 3:
        printk("init key 3 ...\n");
        break;
    }
    return 0;
}

static int imx6ull_gpio_read(int which)
{
    printk("%s %s line %d, \n", __FILE__, __FUNCTION__, __LINE__);

    return 0;
}


static struct key_operations_t mdxz_key_opr =
    {
        .count= 2,
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