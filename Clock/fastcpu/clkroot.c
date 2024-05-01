#include "common.h"
#include "pll.h"
#include "regs.h"
#include "clkroot.h"

extern struct ccm_regs *ccm;

// Initializes and sets the ARM Clock divider (PODF).
void setup_arm_podf(u32 podf) {
    // Validate the divider range (1 to 8).
    if (podf < 1 || podf > 8) return;
    // Set ARM clock frequency divider by writing to CACRR register.
    write32(podf - 1, &ccm->cacrr);
}

// Retrieves the ARM Clock Root frequency.
u32 get_arm_clk_root(void) {
    u32 reg, freq;

    // Read current divider configuration.
    reg = read32(&ccm->cacrr);
    reg = (reg & 0x7) + 1;  // Extract ARM_PODF.
    freq = get_pll(ARM_PLL);  // Retrieve PLL frequency used for ARM Clock.

    return freq / reg;  // Calculate the ARM Clock frequency.
}

// Retrieves the Peripheral Clock frequency.
static u32 get_periph_clk(void) {
    u32 reg, per_clk2_podf = 0, freq = 0;

    reg = read32(&ccm->cbcdr);

    // Determine source of peripheral clock based on PERIPH_CLK_SEL.
    if (reg & (1u << 25)) {
        per_clk2_podf = (reg >> 27) & 0x7;  // Extract PERIPH_CLK2_PODF.
        reg = read32(&ccm->cbcmr);
        switch (reg >> 12 & 0x3) {  // Determine PERIPH_CLK2_SEL source.
            case 0: freq = get_pll(USB1_PLL); break;
            case 1:
            case 2: freq = CKIH; break;  // External oscillator clock.
        }
        freq /= (per_clk2_podf + 1);  // Adjust frequency by the divider.
    } else {
        reg = read32(&ccm->cbcmr);
        switch (reg >> 18 & 0x3) {  // Determine PRE_PERIPH_CLK_SEL source.
            case 0: freq = get_pll(SYS_PLL); break;
            case 1: freq = get_pll_pfd(SYS_PLL, PFD2); break;
            case 2: freq = get_pll_pfd(SYS_PLL, PFD0); break;
            case 3: freq = get_pll_pfd(SYS_PLL, PFD2) / 2; break;
        }
    }

    return freq;
}

// Retrieves the AHB Clock Root frequency.
u32 get_ahb_clk_root(void) {
    u32 reg, ahb_podf;

    reg = read32(&ccm->cbcdr);
    ahb_podf = (reg >> 10) & 0x7;  // Extract AHB_PODF.

    return get_periph_clk() / (ahb_podf + 1);  // Calculate AHB Clock frequency.
}

// Retrieves the IPG Clock Root frequency.
u32 get_ipg_clk_root(void) {
    u32 reg, ipg_podf;

    reg = read32(&ccm->cbcdr);
    ipg_podf = (reg >> 8) & 0x3;  // Extract IPG_PODF.

    return get_ahb_clk_root() / (ipg_podf + 1);  // Calculate IPG Clock frequency.
}

// Retrieves the AXI Clock Root frequency.
u32 get_axi_clk_root(void) {
    u32 root_freq, axi_podf;
    u32 reg = read32(&ccm->cbcdr);

    axi_podf = (reg >> 16) & 0x7;  // Extract AXI_PODF.

    if (reg & (1u << 6)) {
        if (reg & (1u << 7))  // Determine AXI_ALT_SEL.
            root_freq = get_pll_pfd(USB1_PLL, PFD1);
        else
            root_freq = get_pll_pfd(SYS_PLL, PFD2);
    } else
        root_freq = get_periph_clk();  // Default to peripheral clock.

    return root_freq / (axi_podf + 1);  // Calculate AXI Clock frequency.
}

// Retrieves the Fabric MMDC Clock Root frequency.
u32 get_fabric_mmdc_clk_root(void) {
    u32 cbcmr = read32(&ccm->cbcmr);
    u32 cbcdr = read32(&ccm->cbcdr);

    u32 freq, podf, per2_clk2_podf;

    podf = (cbcdr >> 3) & 0x7;  // Extract FABRIC_MMDC_PODF.

    if (cbcdr & (1u << 26)) {
        per2_clk2_podf = cbcdr & 0x7;  // Extract PERIPH2_CLK2_PODF.
        if (cbcmr & (1u << 20))  // Determine PERIPH2_CLK2_SEL.
            freq = CKIH;
        else
            freq = get_pll(USB1_PLL);

        freq /= (per2_clk2_podf + 1);
    } else {
        extern u32 get_pll4_main_clk(void);
        switch ((cbcmr >> 21) & 0x3) {  // Determine PRE_PERIPH2_CLK_SEL.
            case 0: freq = get_pll(SYS_PLL); break;
            case 1: freq = get_pll_pfd(SYS_PLL, PFD2); break;
            case 2: freq = get_pll_pfd(SYS_PLL, PFD0); break;
            case 3: freq = get_pll4_main_clk(); break;
        }
    }

    return freq / (podf + 1);  // Calculate Fabric MMDC Clock frequency.
}
