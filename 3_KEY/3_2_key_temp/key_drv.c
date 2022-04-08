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
#include "key_operation.h"
#include "key_drv.h"
/**************************************DEFINE**************************************/
#define DEVNAME "mdxz_key"

/*********************************The end of define********************************/

/*********************************DECLARE VARIABLE*********************************/

/*1. 确定主设备号；*/
static int key_major = 0; /* default to dynamic major */
static struct class *key_class;
struct key_operations_t *p_key_opr;

/****************************The end of declare variable***************************/

/*********************************DECLARE FUNCTION*********************************/
static int key_drv_read(struct file *filp, char __user *buf, size_t count, loff_t *offset);
static int key_drv_open(struct inode *inode, struct file *filp);
/****************************The end of declare function***************************/

void register_key_opration(struct key_operations_t *opt)
{
    int i;
    p_key_opr = opt;
    for(i = 0; i < opt->count; i++)
        device_create(key_class, NULL, MKDEV(key_major, i), NULL, "mdxz_key%d", i); /* /dev/100ask_led0,1,... */
}

void unregister_key_opration(struct key_operations_t *opt)
{
    int i ;
    for(i = 0; i < opt->count; i++)
        device_destroy(key_class, MKDEV(key_major, i)); /* /dev/100ask_led0,1,... */
}


EXPORT_SYMBOL(register_key_opration);
EXPORT_SYMBOL(unregister_key_opration);

/*2. 定义自己的file\_operations结构体；*/
/*
 * file operations
 */
static const struct file_operations key_ops = {
    .owner = THIS_MODULE,
    .open = key_drv_open,
    .read = key_drv_read,
};

static int key_drv_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static int key_drv_open(struct inode *inode, struct file *filp)
{
    int minor = -1;
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
    minor = iminor(inode);
    p_key_opr->init(minor);

    return 0;
}

/*4. 入口函数：安装驱动时，就会去调用入口函数；*/
static int __init key_drv_init(void)
{
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

    key_major = register_chrdev(0, DEVNAME, &key_ops); // /dev/DEVNAME
    if (key_major < 0)
    {
        printk(DEVNAME ": could not get major number\n");
        return key_major;
    }
    /*class_create*/
    key_class = class_create(THIS_MODULE, "mdxz_key_class");
    if (IS_ERR(key_class))
    {
        class_destroy(key_class);
        unregister_chrdev(key_major, DEVNAME);
        return PTR_ERR(key_class);
    }

    /*get p_led_opr*/
    // p_led_opr = get_board_led_opr();
    return 0;
}
module_init(key_drv_init);

/*5.  出口函数：卸载驱动时，就会去调用出口函数；*/
static void __exit key_drv_exit(void)
{
    printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

    unregister_chrdev(key_major, DEVNAME);
    class_destroy(key_class);
}
module_exit(key_drv_exit);

/*6. 其他完善，提供设备信息，自动创建设备节点。*/
MODULE_LICENSE("GPL");
