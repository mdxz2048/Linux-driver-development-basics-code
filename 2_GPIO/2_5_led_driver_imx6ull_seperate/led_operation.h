#ifndef _LED_OPR_H
#define _LED_OPR_H

 struct led_operations_t{
	int (*init) (int which); /* 初始化LED, which-哪个LED */       
	int (*ctl) (int which, int status); /* 控制LED, which-哪个LED, status:1-亮,0-灭 */
	int (*exit) (int which);
};

struct led_operations_t *get_board_led_opr(void);


#endif

