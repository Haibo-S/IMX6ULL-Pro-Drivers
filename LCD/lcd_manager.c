#include "lcd_controller_manager.h"
#include "lcd_manager.h"
#include "lcd_7_0.h"

#define LCD_NUM 5 /* Maximum number of LCDs that can be registered */

static p_lcd_params p_array_lcd[LCD_NUM]; /* Array to store registered LCD parameters */
static p_lcd_params g_p_lcd_selected;     /* Pointer to the currently selected LCD parameters */

// Provides a simple delay function.
void delay(volatile int d)
{
    while (d--);
}

// Registers an LCD parameter set to the system.
int register_lcd(p_lcd_params plcd)
{
    int i;
    for (i = 0; i < LCD_NUM; i++) {
        if (!p_array_lcd[i]) {
            p_array_lcd[i] = plcd;
            return i; /* Successfully registered */
        }
    }
    return -1; /* No space to register a new LCD */
}

// Selects the active LCD by name.
int select_lcd(char *name)
{
    int i;
    for (i = 0; i < LCD_NUM; i++) {
        if (p_array_lcd[i] && !strcmp(p_array_lcd[i]->name, name)) {
            g_p_lcd_selected = p_array_lcd[i];
            return i; /* LCD found and selected */
        }
    }
    return -1; /* LCD not found */
}

// Retrieves the currently selected LCD parameters.
void get_lcd_params(unsigned int *fb_base, int *xres, int *yres, int *bpp)
{
    *fb_base = g_p_lcd_selected->fb_base;
    *xres = g_p_lcd_selected->xres;
    *yres = g_p_lcd_selected->yres;
    *bpp = g_p_lcd_selected->bpp;
}

// Description: Enables the selected LCD controller.
void lcd_enable(void)
{
    lcd_controller_enable();
}

// Disables the selected LCD controller.
void lcd_disable(void)
{
    lcd_controller_disable();
}

// Initializes the LCD system.
void lcd_init(void)
{
    /* Add LCD parameters for a 7-inch display */
    lcd_7_0_add();
    
    /* Add LCD controller(s) */
    lcd_contoller_add();

    /* Select the IMX6ULL LCD controller */
    select_lcd_controller("Imx6ull");

    /* Select the 7-inch LCD parameters */
    select_lcd("lcd_7.0");

    /* Initialize the selected LCD controller with the selected parameters */
    lcd_controller_init(g_p_lcd_selected);
}
