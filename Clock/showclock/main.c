#include "regs.h"
#include "pll.h"
#include "clkroot.h"

// Define the base address for CCM registers.
struct ccm_regs *ccm = (struct ccm_regs *)CCM_BASE_ADDR;

#define LOOPS 1000000
// Simple busy-wait loop for creating a delay.
static void busy_wait(void) {
    for(u32 i = 0; i < LOOPS; i++); // Loop for a defined number of iterations to create a delay.
}

// External declarations for LED interface functions.
extern void led_init(void);
extern void led_toggle(void);
void led_on(void);

// External declarations for PLL clock settings.
extern void setup_arm_podf(u32 podf);
extern void sel_pll1_sw_clk(int sel_pll1);

// External function for setting up clock output from CLKO1 and CLKO2 pins.
extern void setup_clock_output(void);
#include "uart.h"
#include "my_printf.h"

// Function to display PLL output frequencies and bus root clock frequencies.
void show_clocks(void) {
    u32 freq;

    printf("Show IMX6ULL Clocks: \r\n");
    // Display ARM PLL frequency in MHz.
    freq = get_pll(ARM_PLL);
    printf("ARM_PLL     %8d MHz\r\n", freq / 1000000);

    // Display frequencies of SYS PLL and its PFDs.
    freq = get_pll(SYS_PLL);
    printf("SYS_PLL     %8d MHz\r\n", freq / 1000000);
    freq = get_pll_pfd(SYS_PLL, PFD0);
    printf("|-SYS_PLL_PFD0  %8d MHz\r\n", freq / 1000000);
    freq = get_pll_pfd(SYS_PLL, PFD1);
    printf("|-SYS_PLL_PFD1  %8d MHz\r\n", freq / 1000000);
    freq = get_pll_pfd(SYS_PLL, PFD2);
    printf("|-SYS_PLL_PFD2  %8d MHz\r\n", freq / 1000000);
    freq = get_pll_pfd(SYS_PLL, PFD3);
    printf("|-SYS_PLL_PFD3  %8d MHz\r\n", freq / 1000000);

    // Display frequencies of USB1 PLL and its PFDs.
    freq = get_pll(USB1_PLL);
    printf("USB1_PLL    %8d MHz\r\n", freq / 1000000);
    freq = get_pll_pfd(USB1_PLL, PFD0);
    printf("|-USB1_PLL_PFD0 %8d MHz\r\n", freq / 1000000);
    freq = get_pll_pfd(USB1_PLL, PFD1);
    printf("|-USB1_PLL_PFD1 %8d MHz\r\n", freq / 1000000);
    freq = get_pll_pfd(USB1_PLL, PFD2);
    printf("|-USB1_PLL_PFD2 %8d MHz\r\n", freq / 1000000);
    freq = get_pll_pfd(USB1_PLL, PFD3);
    printf("|-USB1_PLL_PFD3 %8d MHz\r\n", freq / 1000000);

    // Display frequencies of USB2 PLL, AUDIO PLL, and VIDEO PLL.
    freq = get_pll(USB2_PLL);
    printf("USB2_PLL    %8d MHz\r\n", freq / 1000000);
    freq = get_pll(AUDIO_PLL);
    printf("AUDIO_PLL   %8d MHz\r\n", freq / 1000000);
    freq = get_pll(VIDEO_PLL);
    printf("VIDEO_PLL   %8d MHz\r\n", freq / 1000000);

    // Display frequencies of bus root clocks in KHz.
    printf("\r\n");
    freq = get_arm_clk_root();
    printf("ARM_CLK_ROOT    %8d KHz\r\n", freq / 1000);
    freq = get_ahb_clk_root();
    printf("AHB_CLK_ROOT    %8d KHz\r\n", freq / 1000);
    freq = get_ipg_clk_root();
    printf("IPG_CLK_ROOT    %8d KHz\r\n", freq / 1000);
    freq = get_axi_clk_root();
    printf("AXI_CLK_ROOT    %8d KHz\r\n", freq / 1000);
    freq = get_fabric_mmdc_clk_root();
    printf("FABRIC_MMDC_CLK_ROOT    %8d KHz\r\n", freq / 1000);
    printf("\r\n");
}

void main(void) {
    // Initialize and turn on LED.
    int blinks = 0;
    led_init();
    led_on();
    Uart_Init(); // Initialize UART for output.

    sel_pll1_sw_clk(0);  // Switch ARM root clock to oscillator.
    setup_arm_podf(8);  // Set ARM root clock divider to 8.
    set_pll(ARM_PLL, 54);  // Configure ARM_PLL for 648 MHz.
    sel_pll1_sw_clk(1);  // Switch ARM root clock back to ARM_PLL (81 MHz).

    // Blink LED 10 times to observe frequency.
    for (blinks = 10; blinks > 0; blinks--) {
        busy_wait();
        led_toggle();
    }

    sel_pll1_sw_clk(0);  // Switch back to oscillator.
    setup_arm_podf(2);  // Set divider to 2 for higher frequency.
    set_pll(ARM_PLL, 108);  // Configure ARM_PLL for 1296 MHz.
    sel_pll1_sw_clk(1);  // Switch back to ARM_PLL (648 MHz).

    // Display clock frequencies.
    show_clocks();

    // Loop indefinitely, blinking LED at a faster rate.
    while (1) {
        busy_wait();
        led_toggle();
    }
}
