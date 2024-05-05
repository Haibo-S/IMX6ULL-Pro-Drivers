#include "imx6ull_soc.h"
#include "lcd_controller_manager.h"

/* Definitions for backlight GPIO registers */
static volatile unsigned int* GPIO1_IO08_PAD = (volatile unsigned int*)0x20E0308; /* Pad configuration register for backlight pin */
static volatile unsigned int* GPIO1_GDIR = (volatile unsigned int*)0x209C000;     /* GPIO direction register */
static volatile unsigned int* GPIO1_DR = (volatile unsigned int*)0x209C004;       /* GPIO data register */

/* Initialize LCD-related I/O settings */
static void Imx6ull_lcd_io_init(void)
{
    unsigned int* p_tmp;
    unsigned int i = 0;

    /* Initialize SW_MUX settings for 29 signals */
    p_tmp = (unsigned int*)(SW_MUX);
    for (i = 0; i < 29; i++) {
        *p_tmp = 0;  /* Setting 0 selects LCD multiplexing function for each register */
        p_tmp++;
    }

    /* Initialize pad settings for LCD pins */
    p_tmp = (unsigned int*)(SW_PAD);
    for (i = 0; i < 29; i++) {
        *p_tmp = 0xB9;  /* Configure hardware attributes for each LCD pin */
        p_tmp++;
    }

    /* Backlight GPIO initialization */
    *GPIO1_IO08_PAD = 0xB9;     /* Set pad attributes for the backlight pin */
    *GPIO1_GDIR |= (1 << 8);    /* Set backlight GPIO to output mode */
    *GPIO1_DR |= (1 << 8);      /* Turn off the backlight by default */
}

/* Initialize the LCD controller with parameters */
static void Imx6ull_lcd_controller_init(p_lcd_params plcdparams)
{
    int bpp_mode;
    Imx6ull_lcd_io_init();  /* Initialize GPIO pins used by the LCD controller */

    /* Clock calculations simplified: omitting the use of fractional dividers */
    CCM_ANALOG->PLL_VIDEO_NUM = 0; 
    CCM_ANALOG->PLL_VIDEO_DENOM = 0; 

    /* Set PLL_VIDEO for 768MHz output from 24MHz input */
    CCM_ANALOG->PLL_VIDEO = (2 << 19) | (1 << 13) | (32 << 0); 

    /* Set the clock source and dividers for the LCD */
    CCM->CSCDR2 &= ~(7 << 15); /* Clear existing settings */
    CCM->CSCDR2 |= (2 << 15);  /* Set clock source to PLL5_MAIN_CLK */
    CCM->CSCDR2 &= ~(7 << 12);
    CCM->CSCDR2 |= (2 << 12);  /* Set pre-divider to 3 */
    CCM->CSCDR2 &= ~(7 << 9);  /* Set post-divider settings */

    CCM->CBCMR &= ~(7 << 23);                  
    CCM->CBCMR |= (4 << 23);   /* Set the final divider for LCD clock to 5, resulting in 51.2MHz */

    /* Soft reset the LCD controller to synchronize the pixel clock */
    LCDIF->CTRL = 1 << 31;
    delay(100);                /* Delay for controller reset */
    LCDIF->CTRL = 0 << 31;     /* End reset */

    /* Configure the LCD interface */
    bpp_mode = plcdparams->bpp == 8 ? 0x1 : (plcdparams->bpp == 16 ? 0x0 : 0x3); /* Set bpp mode */
    LCDIF->CTRL |= (1 << 19) | (1 << 17) | (3 << 10) | (bpp_mode << 8) | (1 << 5);

    /* Additional settings for 16bpp */
    if (plcdparams->bpp == 16) {
        LCDIF->CTRL |= 1 << 3; /* Set data format to ARGB555 */
    }

    /* Setup frame timings and resolution */
    LCDIF->TRANSFER_COUNT = (plcdparams->yres << 16) | (plcdparams->xres);
    LCDIF->VDCTRL0 = (1 << 28) | (plcdparams->pins_pol.vsync << 27) |
                     (plcdparams->pins_pol.hsync << 26) | (plcdparams->pins_pol.vclk << 25) |
                     (plcdparams->pins_pol.de << 24) | (1 << 21) | (1 << 20) | 
                     (plcdparams->time_seq.tvp);

    LCDIF->VDCTRL1 = plcdparams->time_seq.tvb + plcdparams->time_seq.tvp + 
                     plcdparams->yres + plcdparams->time_seq.tvf;
    LCDIF->VDCTRL2 = (plcdparams->time_seq.thp << 18) | 
                     (plcdparams->time_seq.thb + plcdparams->time_seq.thp + 
                     plcdparams->xres + plcdparams->time_seq.thf);

    /* More setup based on timing */
    LCDIF->VDCTRL3 = ((plcdparams->time_seq.thb + plcdparams->time_seq.thp) << 16) |
                     (plcdparams->time_seq.tvb + plcdparams->time_seq.tvp);
    LCDIF->VDCTRL4 = (1 << 18) | (plcdparams->xres);

    /* Frame buffer setup */
    LCDIF->CUR_BUF = plcdparams->fb_base;
    LCDIF->NEXT_BUF = plcdparams->fb_base;

    /* Enable the controller */
    LCDIF->CTRL |= 1 << 0;
}

/* Enable the LCD controller */
static void Imx6ull_lcd_controller_enable(void)
{
    LCDIF->CTRL |= 1 << 0; /* Enable the LCD controller */
}

/* Disable the LCD controller */
static void Imx6ull_lcd_controller_disable(void)
{
    LCDIF->CTRL &= ~(1 << 0); /* Disable the LCD controller */
}

/* LCD controller definition */
struct lcd_controller Imx6ull_lcd_controller = {
    .name = "Imx6ull",
    .init = Imx6ull_lcd_controller_init,
    .enable = Imx6ull_lcd_controller_enable,
    .disable = Imx6ull_lcd_controller_disable,
};

/* Register the IMX6ULL LCD controller */
void Imx6ull_lcd_controller_add(void)
{
    register_lcd_controller(&Imx6ull_lcd_controller);
}
