#include "common.h"
#include "pll.h"
#include "regs.h"
#include "clkroot.h"

extern struct ccm_regs *ccm;

// Configures the divider for ARM_CLK_ROOT.
void setup_arm_podf(u32 podf) {
    // Validate that the input divider is within the allowable range (1 to 8).
    if (podf < 1 || podf > 8) return;
    // Write the divider setting to the CACRR register, adjusting by -1 for 0-based index.
    write32(podf - 1, &ccm->cacrr);
}

// Retrieves the current frequency of ARM_CLK_ROOT.
u32 get_arm_clk_root(void) {
    u32 reg, freq;

    // Read the current divider setting from CACRR register.
    reg = read32(&ccm->cacrr);
    reg = (reg & 0x7) + 1;  // Adjust divider from 0-based to actual value.
    freq = get_pll(ARM_PLL);  // Get the frequency of ARM_PLL.

    return freq / reg;  // Calculate the ARM_CLK_ROOT frequency.
}

// Retrieves the current frequency of PERIPH_CLK.
static u32 get_periph_clk(void) {
    u32 reg, per_clk2_podf = 0, freq = 0;

    reg = read32(&ccm->cbcdr);

    // Determine the clock source for PERIPH_CLK based on PERIPH_CLK_SEL.
    if (reg & (1u << 25)) {  // If periph_clk2 is selected.
        per_clk2_podf = (reg >> 27) & 0x7;  // Extract the divider for PERIPH_CLK2_PODF.
        reg = read32(&ccm->cbcmr);
        // Check which clock is selected as the source for PERIPH_CLK2.
        switch (reg >> 12 & 0x3) {
            case 0: freq = get_pll(USB1_PLL); break;
            case 1:
            case 2: freq = CKIH; break;  // Oscillator clock.
        }
        freq /= (per_clk2_podf + 1);  // Adjust the frequency by the divider value.
    } else {  // If pre_periph_clk is selected.
        reg = read32(&ccm->cbcmr);
        // Determine the source for pre_periph_clk.
        switch (reg >> 18 & 0x3) {
            case 0: freq = get_pll(SYS_PLL); break;
            case 1: freq = get_pll_pfd(SYS_PLL, PFD2); break;
            case 2: freq = get_pll_pfd(SYS_PLL, PFD0); break;
            case 3: freq = get_pll_pfd(SYS_PLL, PFD2) / 2; break;
        }
    }

    return freq;
}

// Retrieves the current frequency of AHB_CLK_ROOT.
u32 get_ahb_clk_root(void) {
    u32 reg, ahb_podf;

    // Read the AHB_PODF divider setting from CBCDR register.
    reg = read32(&ccm->cbcdr);
    ahb_podf = (reg >> 10) & 0x7;

    return get_periph_clk() / (ahb_podf + 1);  // Calculate the AHB_CLK_ROOT frequency.
}

// Retrieves the current frequency of IPG_CLK_ROOT.
u32 get_ipg_clk_root(void) {
    u32 reg, ipg_podf;

    // Read the IPG_PODF divider setting from CBCDR register.
    reg = read32(&ccm->cbcdr);
    ipg_podf = (reg >> 8) & 0x3;

    return get_ahb_clk_root() / (ipg_podf + 1);  // Calculate the IPG_CLK_ROOT frequency.
}

// Retrieves the current frequency of AXI_CLK_ROOT.
u32 get_axi_clk_root(void) {
    u32 root_freq, axi_podf;
    u32 reg = read32(&ccm->cbcdr);

    // Read the AXI_PODF divider setting.
    axi_podf = (reg >> 16) & 0x7;

    // Determine the source for AXI_CLK based on AXI_SEL.
    if (reg & (1u << 6)) {  // If axi_alt_clk is selected.
        // Check AXI_ALT_SEL for the specific alternate clock source.
        if (reg & (1u << 7))
            root_freq = get_pll_pfd(USB1_PLL, PFD1);
        else
            root_freq = get_pll_pfd(SYS_PLL, PFD2);
    } else
        root_freq = get_periph_clk();  // Default to periph_clk.

    return root_freq / (axi_podf + 1);  // Calculate the AXI_CLK_ROOT frequency.
}

// Retrieves the current frequency of FABRIC_MMDC_CLK_ROOT.
u32 get_fabric_mmdc_clk_root(void) {
    u32 cbcmr = read32(&ccm->cbcmr);
    u32 cbcdr = read32(&ccm->cbcdr);

    u32 freq, podf, per2_clk2_podf;

    // Read the FABRIC_MMDC_PODF divider setting.
    podf = (cbcdr >> 3) & 0x7;

    // Determine the clock source for FABRIC_MMDC_CLK based on PERIPH2_CLK_SEL.
    if (cbcdr & (1u << 26)) {  // If periph2_clk2 is selected.
        per2_clk2_podf = cbcdr & 0x7;  // Extract the divider for PERIPH2_CLK2_PODF.
        // Check PERIPH2_CLK2_SEL for the specific clock source.
        if (cbcmr & (1u << 20))
            freq = CKIH;
        else
            freq = get_pll(USB1_PLL);

        freq /= (per2_clk2_podf + 1);  // Adjust the frequency by the divider value.
    } else {  // If pre_periph2_clk is selected.
        extern u32 get_pll4_main_clk(void);
        // Determine the source for pre_periph2_clk.
        switch ((cbcmr >> 21) & 0x3) {
            case 0: freq = get_pll(SYS_PLL); break;
            case 1: freq = get_pll_pfd(SYS_PLL, PFD2); break;
            case 2: freq = get_pll_pfd(SYS_PLL, PFD0); break;
            case 3: freq = get_pll4_main_clk(); break;
        }
    }

    return freq / (podf + 1);  // Calculate the FABRIC_MMDC_CLK_ROOT frequency.
}
