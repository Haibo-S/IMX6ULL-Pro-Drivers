#include "regs.h"
#include "pll.h"

extern struct ccm_regs *ccm;

/**
 * Switches the clock source for PLL1_SW_CLK.
 * 
 * @param sel_pll1 If 0, selects XTALOSC24M output; if 1, selects PLL1 output.
 */
void sel_pll1_sw_clk(int sel_pll1) {
    // Toggle between pll1_main_clk and step_clk based on input.
    if (sel_pll1) {
        clr_bit(&ccm->ccsr, 2); // Select pll1_main_clk.
    } else {
        clr_bit(&ccm->ccsr, 8); // step_clk uses OSC output.
        set_bit(&ccm->ccsr, 2); // Select step_clk.
    }
}

/**
 * Retrieves the clock frequency of PLL1_SW_CLK.
 * 
 * @return The frequency of PLL1_SW_CLK.
 */
u32 get_pll1_sw_clk(void) {
    u32 reg = read32(&ccm->ccsr);

    if (reg & (1u << 2)) { // Check if pll1_main_clk or step_clk.
        if (reg & (1u << 8)) { // Check if secondary_clk or OSC.
            if (reg & (1u << 3)) // Determine if PLL2 or PLL2 PFD2.
                return get_pll(SYS_PLL);
            else
                return get_pll_pfd(SYS_PLL, PFD2);
        } else {
            return CKIH; // OSC output.
        }
    } else {
        return get_pll(ARM_PLL);
    }
}

/**
 * Retrieves the clock frequency of PLL3_SW_CLK.
 * 
 * @return The frequency of PLL3_SW_CLK.
 */
u32 get_pll3_sw_clk(void) {
    u32 reg = read32(&ccm->ccsr);
    if (reg & 1) { // Check if pll3 or pll3_bypass (OSC output).
        return get_pll(USB1_PLL);
    } else {
        return CKIH; // OSC output.
    }
}

/**
 * Retrieves the clock frequency of PLL4_MAIN_CLK.
 * 
 * @return The frequency of PLL4_MAIN_CLK.
 */
extern struct anadig_regs *anadig;
u32 get_pll4_main_clk(void) {
    u32 reg, audio_div;

    reg = read32(&anadig->pmu_misc2);
    // Determine the audio division factor based on MSB and LSB.
    audio_div = reg & (1u << 15) ? (reg & (1u << 23) ? 4 : 2) : 1;

    return get_pll(AUDIO_PLL) / audio_div;
}

/**
 * Retrieves the clock frequency of PLL5_MAIN_CLK.
 * 
 * @return The frequency of PLL5_MAIN_CLK.
 */
u32 get_pll5_main_clk(void) {
    u32 reg, video_div;

    reg = read32(&anadig->pmu_misc2);
    // Determine the video division factor based on MSB and LSB.
    video_div = reg & (1u << 30) ? (reg & (1u << 31) ? 4 : 2) : 1;

    return get_pll(VIDEO_PLL) / video_div;
}
