#include "lcd_manager.h"

void lcd_test(void){
	unsigned int fb_base ;
	int xres, yres, bpp;
	int x, y;
	unsigned short *p;
	unsigned int *p2;
	
	lcd_enable();

	get_lcd_params(&fb_base, &xres, &yres, &bpp);

	if (bpp == 16)
	{
		/* ARGB555 */
		/*red*/
		p = (unsigned short *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p++ = 0x7c00;

		/* green */
		p = (unsigned short *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p++ = 0x3E0;

		/* blue */
		p = (unsigned short *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p++ = 0x1f;
			
	}
	else if (bpp == 32)
	{
		/* ARGB888 */
		/*red*/
		p2 = (unsigned int *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p2++ = 0xff0000;

		/*green*/ 
		p2 = (unsigned int *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p2++ = 0x00ff00;

		/*blue*/ 
		p2 = (unsigned int *)fb_base;
		for (x = 0; x < xres; x++)
			for (y = 0; y < yres; y++)
				*p2++ = 0x0000ff;

	}
}



int main(void) {
	lcd_init();

	lcd_test();

}

