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
#include <linux/platform_device.h>
#include "led_resource.h"

static struct led_resource led_resources = {
    .pin = GROUP_PIN(3,1),

};

struct led_resource  *get_led_resouce(void)
{
    return &led_resources;
}
