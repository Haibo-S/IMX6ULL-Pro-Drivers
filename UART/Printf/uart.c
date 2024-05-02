#include "uart.h"

// Define pointers to UART pin configuration registers.
static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA;
static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA;

/**
 * Initializes UART1 with predefined transmission format and baud rate settings.
 * Configures the GPIO pins used by UART1 for transmission and reception.
 */
void Uart_Init(void) {
    // Set addresses for the control pads of UART1 TX and RX.
    IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA = (volatile unsigned int *)(0x20E0084);
    IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA = (volatile unsigned int *)(0x20E0088);

    // Reset configurations for both TX and RX control pads.
    *IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA = 0;
    *IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA = 0;

    // Disable UART1 before configuration.
    UART1->UCR1 |= (1 << 0); 

    // Configure UART transmission format:
    // Ignore RTS pin, set data length to 8 bits, enable transmitter and receiver.
    UART1->UCR2 |= (1 << 14) | (1 << 5) | (1 << 2) | (1 << 1);

    // Set the additional control features for UART1.
    UART1->UCR3 |= (1 << 2);  // According to the documentation for IM6ULL, set MUXED model.

    // Configure baud rate:
    // Use a reference frequency of 80MHz to set baud rate to 115200.
    UART1->UFCR = 5 << 7;  // Set UART clock to 80MHz.
    UART1->UBIR = 71;      // UBIR value.
    UART1->UBMR = 3124;    // UBMR value, together achieving the desired baud rate.

    // Re-enable UART1 after configuration.
    UART1->UCR1 |= (1 << 0);
}

/**
 * Sends a single character over UART1.
 * 
 * @param c The character to send.
 */
void PutChar(int c) {
    // Wait until the previous character has been transmitted.
    while (!(UART1->USR2 & (1 << 3)));

    // Transmit the new character.
    UART1->UTXD = (unsigned char)c;
}

/**
 * Receives a single character from UART1.
 * 
 * @return The character received.
 */
unsigned char GetChar(void) {
    // Wait until there is data to read.
    while (!(UART1->USR2 & (1 << 0)));

    // Return the character read.
    return (unsigned char)UART1->URXD;
}

/**
 * Sends a string over UART1.
 * 
 * @param s The string to send.
 */
void PutStr(const char *s) {
    while (*s) {
        PutChar(*s++);
    }
}

int raise(int signum) {
    return 0;
}
		  			 		  						  					  				 	   		  	  	 	  