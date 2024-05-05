#include "lcd_controller_manager.h"
#include "lcd_manager.h"
#include "imx6ull_con.h"

#define LCD_CONTROLLER_NUM 10 /* Maximum number of LCD controllers that can be registered */

static p_lcd_controller p_array_lcd_controller[LCD_CONTROLLER_NUM]; /* Array to store registered LCD controllers */
static p_lcd_controller g_p_lcd_controller_selected; /* Pointer to the currently selected LCD controller */

// Compares two strings.
int strcmp(const char *cs, const char *ct)
{
    register signed char __res;

    while (1) {
        if ((__res = *cs - *ct++) != 0 || !*cs++)
            break;
    }

    return __res;
}


// Registers an LCD controller to the system.
int register_lcd_controller(p_lcd_controller plcdcon)
{
    int i;
    for (i = 0; i < LCD_CONTROLLER_NUM; i++) {
        if (!p_array_lcd_controller[i]) {
            p_array_lcd_controller[i] = plcdcon;
            return i; /* Successfully registered */
        }
    }
    return -1; /* No space to register a new controller */
}


// Selects the active LCD controller by name.
int select_lcd_controller(char *name)
{
    int i;
    for (i = 0; i < LCD_CONTROLLER_NUM; i++) {
        if (p_array_lcd_controller[i] && !strcmp(p_array_lcd_controller[i]->name, name)) {
            g_p_lcd_controller_selected = p_array_lcd_controller[i];
            return i; /* Controller found and selected */
        }
    }
    return -1; /* Controller not found */
}

// Description: Initializes the selected LCD controller. 
int lcd_controller_init(p_lcd_params plcdparams)
{
    if (g_p_lcd_controller_selected) {
        g_p_lcd_controller_selected->init(plcdparams);
        return 0; /* Initialization successful */
    }
    return -1; /* No controller selected */
}


// Description: Enables the selected LCD controller.
void lcd_controller_enable(void)
{
    if (g_p_lcd_controller_selected) {
        g_p_lcd_controller_selected->enable();
    }
}


// Disables the selected LCD controller.
void lcd_controller_disable(void)
{
    if (g_p_lcd_controller_selected) {
        g_p_lcd_controller_selected->disable();
    }
}

// Adds the default Imx6ull LCD controller to the system.
void lcd_controller_add(void)
{
    Imx6ull_lcd_controller_add(); /* Register the default IMX6ULL LCD controller */
}
