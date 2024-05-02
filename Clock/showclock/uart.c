#include "uart.h"

static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA;
static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA;

/**
 * Initializes the UART by setting transmission formats and baud rate to 115200.
 * Configures the UART GPIO pins.
 */
void Uart_Init(void) {
    // Set addresses for control pads.
    IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA = (volatile unsigned int *)(0x20E0084);
    IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA = (volatile unsigned int *)(0x20E0088);

    // Clear configurations.
    *IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA = 0;
    *IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA = 0;

    // Disable UART before configuring.
    UART1->UCR1 |= (1 << 0); 

    // Configure UART transmission format settings.
    UART1->UCR2 |= (1<<14) | (1<<5) | (1<<2) | (1<<1); // Enable various transmission parameters.

    // Set additional UART configuration.
    UART1->UCR3 |= (1<<2); // Set MUXED model flag.

    // Set the baud rate.
    UART1->UFCR = 5 << 7;  // Reference frequency for UART clock: 80MHz.
    UART1->UBIR = 71;      // Set UBIR value.
    UART1->UBMR = 3124;    // Set UBMR value, configures baud rate to 115200.

    // Re-enable UART.
    UART1->UCR1 |= (1 << 0);
}

/**
 * Sends a single character to the UART.
 * 
 * @param c The character to send.
 */
void PutChar(int c) {
    while (!((UART1->USR2) & (1<<3))); // Wait for the previous byte to finish transmitting.
    UART1->UTXD = (unsigned char)c;   // Send the character.
}

/**
 * Receives a single character from UART.
 * 
 * @return The received character.
 */
unsigned char GetChar(void) {
    while (!(UART1->USR2 & (1<<0))); // Wait for data to be available.
    return (unsigned char)UART1->URXD; // Return the received data.
}

/**
 * Sends a string over UART by sending each character in sequence.
 * 
 * @param s The string to send.
 */
void PutStr(const char *s) {
    while (*s) {
        PutChar(*s++);
    }
}

/**
 * Function to handle unsupported operations or signals, often required for linking.
 * 
 * @param signum Signal number.
 * @return Always returns 0 indicating no action taken.
 */
int raise(int signum) {
    return 0; // No operation performed, used to avoid compilation errors.
}
