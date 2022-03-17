#ifndef _LED_DRIVE_H
#define _LED_DRIVE_H

void led_class_create_device(int minor);
void led_destroy_device(int minor);
int register_led_operations(struct led_operations_t *led_opr);



#endif

