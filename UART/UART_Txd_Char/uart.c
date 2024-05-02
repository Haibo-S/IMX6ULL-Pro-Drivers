#include "uart.h"

// Pointers to IOMUX configuration registers for UART1 TX and RX data.
static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA;
static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA;

/**
 * Initializes UART1 with a specific transmission format and baud rate.
 * This function configures UART1 for 115200 baud rate, 8N1 format, and
 * sets up the necessary GPIO pins for UART functionality.
 */
void Uart_Init(void) {
    // Initialize the address pointers for UART1 TX and RX pins.
    IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA = (volatile unsigned int *)(0x20E0084);
    IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA = (volatile unsigned int *)(0x20E0088);

    // Reset configurations for TX and RX pins.
    *IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA = 0;
    *IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA = 0;

    // Disable UART1 before making any changes.
    UART1->UCR1 |= (1 << 0);  // UCR1[0] = 1 (UART disabled)

    // Configure UART1 control settings.
    UART1->UCR2 |= (1 << 14) | (1 << 5) | (1 << 2) | (1 << 1);  // Ignore RTS, 8-bit data, enable TX, enable RX

    // Set the MUXED model for UART according to the documentation.
    UART1->UCR3 |= (1 << 2);  // Enable MUXED model.

    // Configure the baud rate.
    UART1->UFCR = 5 << 7;  // Set the ref freq ratio (divider 5 implies 80 MHz / 5 = 16 MHz).
    UART1->UBIR = 71;      // UBIR value as per calculation.
    UART1->UBMR = 3124;    // UBMR value as per calculation.

    // Re-enable UART1 after configuration.
    UART1->UCR1 |= (1 << 0);  // UCR1[0] = 1 (UART enabled)
}

/**
 * Transmits a single character over UART1.
 * 
 * @param c Character to be transmitted.
 */
void PutChar(int c) {
    // Wait for the transmitter buffer to be empty.
    while (!(UART1->USR2 & (1 << 3)));  // USR2[3] = 1 when transmit buffer is empty.

    // Write the character to the transmit data register.
    UART1->UTXD = (unsigned char)c;
}
