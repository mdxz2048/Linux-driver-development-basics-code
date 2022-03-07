#ifndef _LED_DRIVE_H
#define _LED_DRIVE_H

int led_class_create_device(int minor);
int led_destroy_device(int minor);
int register_led_operations(led_operations_t *led_opr);



#endif

