#ifndef _LCD_MANAGER_H_
#define _LCD_MANAGER_H_

enum {
	NORMAL = 0,
	INVERT = 1,
};

typedef struct pins_polarity {
	int de;    
	int vclk;  
	int hsync;
	int vsync; 
}pins_polarity, *p_pins_polarity;

typedef struct time_sequence {
	int tvp; 
	int tvb; 
	int tvf; 

	int thp; 
	int thb;
	int thf; 

	int vclk;
}time_sequence, *p_time_sequence;


typedef struct lcd_params {
	
	char *name;
	
	pins_polarity pins_pol;

	time_sequence time_seq;

	int xres;
	int yres;
	int bpp;

	unsigned int fb_base;
	
}lcd_params, *p_lcd_params;

void delay(volatile int d);

int register_lcd(p_lcd_params plcd);

void get_lcd_params(unsigned int *fb_base, int *xres, int *yres, int *bpp);

void lcd_enable(void);

void lcd_disable(void);

void lcd_init(void);

#endif 


