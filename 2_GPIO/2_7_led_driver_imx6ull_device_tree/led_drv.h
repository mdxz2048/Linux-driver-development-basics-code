#ifndef _LED_DRIVE_H
#define _LED_DRIVE_H

void led_class_create_device(int minor);
void led_class_destroy_device(int minor);
void register_led_options(struct led_operations_t *opr);

#endif
