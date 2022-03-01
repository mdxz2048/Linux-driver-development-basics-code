#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <asm/io.h>

#define DEVNAME "mdxz_led"



static int led_open(struct inode *inode, struct file *filp);
static int led_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos);
/*REGISTER ADDRESS */
static volatile uint32_t *SW_MUX_CTL_PAD_SNVS_TAMPER3; // 2290014h
static volatile uint32_t *GPIO5_GDIR;				   // 20AC004h
static volatile uint32_t *GPIO5_DR;					   // 20AC000h

/*1. 确定主设备号；*/
static int led_major; /* default to dynamic major */
static struct class *led_class;

/*2. 定义自己的file\_operations结构体；*/
/*
 * file operations
 */
static const struct file_operations led_ops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.write = led_write,
};

/*3.  实现对应的open/read/write等函数，填入file\_operations结构体；*/
static int led_open(struct inode *inode, struct file *filp)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	/*CONFIG GPIO */
	*SW_MUX_CTL_PAD_SNVS_TAMPER3 &= ~0xf; // clear
	*SW_MUX_CTL_PAD_SNVS_TAMPER3 |= 0x5;  // set

	/*CONFIG GPIO OUTPUT*/
	*GPIO5_GDIR |= (1 << 3);
	return 0;
}

static int led_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos)
{
	char value = 0;
	int ret = -1;
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

	ret = copy_from_user(&value, buf, 1);
	if (value == 0)
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

/*4. 入口函数：安装驱动时，就会去调用入口函数；*/
static int __init led_init(void)
{
	int i = 0;
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	/*ioremap*/
	SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x2290014, 4);
	GPIO5_GDIR = ioremap(0x20AC004, 4);
	GPIO5_DR = ioremap(0x20AC000, 4);
	/*class_create*/
	led_class = class_create(THIS_MODULE, "mdxz_led");
	if (IS_ERR(led_class))
		return PTR_ERR(led_class);
	led_major = register_chrdev(0, DEVNAME, &led_ops);
	if (led_major < 0)
	{
		printk(DEVNAME
			   ": could not get major number\n");
		class_destroy(led_class);
		return led_major;
	}

	device_create(led_class, NULL, MKDEV(led_major, 0), NULL, "mdxz_led%d", i);

	return 0;
}
/*5.  出口函数：卸载驱动时，就会去调用出口函数；*/
static void __exit led_exit(void)
{
	iounmap(SW_MUX_CTL_PAD_SNVS_TAMPER3);
	iounmap(GPIO5_GDIR);
	iounmap(GPIO5_DR);
	unregister_chrdev(led_major, DEVNAME);
	class_destroy(led_class);
	device_destroy(led_class, MKDEV(led_major, 0));
}
/*6. 其他完善，提供设备信息，自动创建设备节点。*/
module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
