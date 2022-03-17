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
#include "led_drv.h"
/**************************************DEFINE**************************************/
#define DEVNAME "mdxz_led"

/*********************************The end of define********************************/

/*********************************DECLARE VARIABLE*********************************/

struct led_operations_t *p_led_opr;
/*1. 确定主设备号；*/
static int led_major; /* default to dynamic major */
static struct class *led_class;

/****************************The end of declare variable***************************/

/*********************************DECLARE FUNCTION*********************************/
static int led_drv_open(struct inode *inode, struct file *filp);
static int led_drv_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos);
static int led_drv_close(struct inode *node, struct file *file);
/****************************The end of declare function***************************/
int led_class_create_device(int minor)
{
    device_create(led_class, NULL, MKDEV(led_major, 0), NULL, "mdxz_led%d", minor); ///* /dev/mdxz_led0,1,... */

    return 0;
}
int led_destroy_device(int minor)
{
    device_destroy(led_class, MKDEV(led_major, minor));
    return 0;
}

int register_led_operations(struct led_operations_t *led_opr)
{
    p_led_opr = led_opr;
    return 0;
}
EXPORT_SYMBOL(led_class_create_device);
EXPORT_SYMBOL(led_destroy_device);
EXPORT_SYMBOL(register_led_operations);

/*2. 定义自己的file\_operations结构体；*/
/*
 * file operations
 */
static const struct file_operations led_ops = {
    .owner = THIS_MODULE,
    .open = led_drv_open,
    .write = led_drv_write,
    .release = led_drv_close,
};


/*3.  实现对应的open/read/write等函数，填入file\_operations结构体；*/
static int led_drv_open(struct inode *inode, struct file *filp)
{
    int minor = -1;
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

    minor = iminor(inode);
    p_led_opr->init(minor);

    return 0;
}

static int led_drv_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos)
{
    char value = 0;
    int ret = -1;
    // printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

    struct inode *inode = file_inode(filp);
    int minor = iminor(inode);
    ret = copy_from_user(&value, buf, 1);

    p_led_opr->ctl(minor, value);

    return 0;
}

static int led_drv_close(struct inode *node, struct file *file)
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    /*提取次设备号*/

    return 0;
}

/*4. 入口函数：安装驱动时，就会去调用入口函数；*/
static int __init led_drv_init(void)
{
    // int i = 0;
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    /*class_create*/
    led_class = class_create(THIS_MODULE, "mdxz_led");
    if (IS_ERR(led_class))
        return PTR_ERR(led_class);
    /*get p_led_opr*/
    p_led_opr = get_board_led_opr();

    led_major = register_chrdev(0, DEVNAME, &led_ops);
    if (led_major < 0)
    {
        printk(DEVNAME ": could not get major number\n");
        class_destroy(led_class);
        return led_major;
    }

    return 0;
}
module_init(led_drv_init);

/*5.  出口函数：卸载驱动时，就会去调用出口函数；*/
static void __exit led_drv_exit(void)
{
    unregister_chrdev(led_major, DEVNAME);
    class_destroy(led_class);
}
module_exit(led_drv_exit);

/*6. 其他完善，提供设备信息，自动创建设备节点。*/
MODULE_LICENSE("GPL");
