#include "regs.h"
#include "pll.h"
#include "clkroot.h"

// Define the base address for the CCM registers.
struct ccm_regs *ccm = (struct ccm_regs *)CCM_BASE_ADDR;

// Define the number of loops for the busy wait function.
#define LOOPS 1000000

// Function to perform a busy wait for a simple delay.
static void busy_wait(void) {
    for (u32 i = 0; i < LOOPS; i++);  // Loop to create a delay.
}

// LED control functions
extern void led_init(void);    // Initializes LED
extern void led_toggle(void);  // Toggles LED state
void led_on(void);             // Prototype for turning the LED on

// Functions to control PLL and clock settings
extern void setup_arm_podf(u32 podf);    // Set ARM clock divider
extern void sel_pll1_sw_clk(int sel_pll1);  // Select PLL1 switch clock

// Main function contains the core logic for initializing and manipulating the LED and PLL settings.
void main(void) {
    int blinks = 0;   // Counter for LED blinks

    led_init();   // Initialize LED
    led_on();     // Turn on LED

    sel_pll1_sw_clk(0);     // Switch ARM root clock to OSC (oscillator)
    setup_arm_podf(8);      // Set ARM root clock divider to 8
    set_pll(ARM_PLL, 54);   // Configure ARM_PLL for 648 MHz (24 * 54 / 2)
    sel_pll1_sw_clk(1);     // Switch ARM root clock back to ARM_PLL at 81 MHz

    // Loop to toggle LED 10 times to observe blinking rate
    for (blinks = 10; blinks > 0; blinks--) {
        busy_wait();    // Delay between toggles
        led_toggle();   // Toggle LED state
    }

    sel_pll1_sw_clk(0);     // Switch ARM root clock back to OSC
    setup_arm_podf(2);      // Set ARM root clock divider to 2
    set_pll(ARM_PLL, 108);  // Configure ARM_PLL for 1296 MHz (24 * 108 / 2)
    sel_pll1_sw_clk(1);     // Switch ARM root clock back to ARM_PLL at 648 MHz

    // Infinite loop to toggle LED, observing increased blink frequency
    while (1) {
        busy_wait();    // Delay between toggles
        led_toggle();   // Toggle LED state
    }
}

// Required by GCC for division operations, providing a raise function for normal compilation.
void raise(void) {}
