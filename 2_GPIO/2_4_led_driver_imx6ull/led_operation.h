#ifndef _LED_OPR_H
#define _LED_OPR_H

typedef struct {
	int (*init) (void); /* 初始化LED, which-哪个LED */       
	int (*ctl) (int status); /* 控制LED, which-哪个LED, status:1-亮,0-灭 */
	int (*exit) (void);
}led_operations_t;

led_operations_t *get_board_led_opr(void);


#endif

