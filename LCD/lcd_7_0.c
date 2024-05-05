#include "lcd_manager.h"

#define LCD_FB_BASE   0x99000000 /* Base address for the frame buffer */

/* LCD parameters for a 7.0 inch display */
lcd_params lcd_7_0_params = {
    .name = "lcd_7.0",
    .pins_pol = {
        .de    = INVERT,   /* DE signal is active low */
        .vclk  = INVERT,   /* Fetch data at the falling edge of the clock */
        .hsync = NORMAL,   /* HSYNC active low pulse */
        .vsync = NORMAL,   /* VSYNC active low pulse */
    },
    .time_seq = {
        /* Vertical timing parameters */
        .tvp = 3,    /* VSYNC pulse width */
        .tvb = 20,   /* Vertical back porch */
        .tvf = 12,   /* Vertical front porch */

        /* Horizontal timing parameters */
        .thp = 20,   /* HSYNC pulse width */
        .thb = 140,  /* Horizontal back porch */
        .thf = 160,  /* Horizontal front porch */

        .vclk = 51.2,  /* Pixel Clock frequency in MHz */
    },
    .xres = 1024,      /* Horizontal resolution in pixels */
    .yres = 600,       /* Vertical resolution in pixels */
    .bpp  = 16,        /* Bits per pixel */
    .fb_base = LCD_FB_BASE,  /* Frame buffer base address */
};

// Registers the 7-inch LCD parameters with the LCD manager.
void lcd_7_0_add(void)
{
    register_lcd(&lcd_7_0_params);
}
