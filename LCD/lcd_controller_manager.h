#ifndef _LCD_CONTROLLER_MANAGER_H_
#define _LCD_CONTROLLER_MANAGER_H_

#include "lcd_manager.h"

typedef struct lcd_controller{
	char* name;
	void (*init)(p_lcd_params plcdparams);
	void(*enable)(void);
	void(*disable)(void);
}lcd_controller, *p_lcd_controller;

void lcd_contoller_add(void);

int lcd_controller_init(p_lcd_params plcdparams);

void lcd_controller_enable(void);

void lcd_controller_disable(void);

int register_lcd_controller(p_lcd_controller plcdcon);

int select_lcd_controller(char *name);

int strcmp(const char * cs,const char * ct);

#endif

