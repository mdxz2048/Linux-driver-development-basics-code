#ifndef _KEY_OPR_H
#define _KEY_OPR_H

 struct key_operations_t{
	int count;    
	int (*init) (int which); 
	int (*read) (int which);
};

struct led_operations_t *get_board_led_opr(void);


#endif

