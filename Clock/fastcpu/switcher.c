#include "regs.h"
#include "pll.h"

extern struct ccm_regs *ccm;

// Sets the clock source for PLL1_SW_CLK.
void sel_pll1_sw_clk(int sel_pll1) {
    // Select clock source based on input parameter sel_pll1
    if (sel_pll1) {
        // Set PLL1 as the main clock (clear bit to use pll1_main_clk).
        clr_bit(&ccm->ccsr, 2);
    } else {
        // Set OSC (oscillator) as the step clock source.
        clr_bit(&ccm->ccsr, 8);
        // Enable step clock (set bit to select step_clk).
        set_bit(&ccm->ccsr, 2);
    }
}

// Retrieves the frequency of PLL1_SW_CLK.
u32 get_pll1_sw_clk(void) {
    u32 reg = read32(&ccm->ccsr);
    // Check current source of PLL1_SW_CLK.
    if (reg & (1u << 2)) {  // If using step_clk
        if (reg & (1u << 8)) {  // Check if secondary_clk is selected
            // Choose between SYS_PLL and its PFD2 based on SECONDARY_CLK_SEL.
            if (reg & (1u << 3))
                return get_pll(SYS_PLL);
            else
                return get_pll_pfd(SYS_PLL, PFD2);
        } else {
            return CKIH;  // Oscillator output
        }
    } else {
        return get_pll(ARM_PLL);  // Return frequency of ARM_PLL if pll1_main_clk is selected.
    }
}

// Retrieves the frequency of PLL3_SW_CLK.
u32 get_pll3_sw_clk(void) {
    u32 reg = read32(&ccm->ccsr);
    // Check if pll3 or pll3_bypass (OSC output) is used for PLL3_SW_CLK.
    if (reg & 1)  // PLL3 selected
        return get_pll(USB1_PLL);
    else
        return CKIH;  // OSC output
}

// Retrieves the frequency of PLL4_MAIN_CLK.
extern struct anadig_regs *anadig;
u32 get_pll4_main_clk(void) {
    u32 reg, audio_div;
    reg = read32(&anadig->pmu_misc2);
    // Determine audio division based on AUDIO_DIV_MSB and AUDIO_DIV_LSB.
    audio_div = reg & (1u << 15) ? (reg & (1u << 23) ? 4 : 2) : 1;
    return get_pll(AUDIO_PLL) / audio_div;  // Return PLL4_MAIN_CLK frequency after division.
}

// Retrieves the frequency of PLL5_MAIN_CLK.
u32 get_pll5_main_clk(void) {
    u32 reg, video_div;
    reg = read32(&anadig->pmu_misc2);
    // Determine video division based on AUDIO_DIV_MSB and AUDIO_DIV_LSB.
    video_div = reg & (1u << 30) ? (reg & (1u << 31) ? 4 : 2) : 1;
    return get_pll(VIDEO_PLL) / video_div;  // Return PLL5_MAIN_CLK frequency after division.
}
