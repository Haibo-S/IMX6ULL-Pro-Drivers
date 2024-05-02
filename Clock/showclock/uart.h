#ifndef _UART_H_
#define _UART_H_

// Base address for UART1.
#define UART1_BASE          (0x2020000u)

// Structure to represent the UART registers.
typedef struct {
    volatile unsigned int  URXD;          // UART Receiver Register, offset: 0x0
    unsigned char RESERVED_0[60];
    volatile unsigned int  UTXD;          // UART Transmitter Register, offset: 0x40
    unsigned char RESERVED_1[60];
    volatile unsigned int  UCR1;          // UART Control Register 1, offset: 0x80
    volatile unsigned int  UCR2;          // UART Control Register 2, offset: 0x84
    volatile unsigned int  UCR3;          // UART Control Register 3, offset: 0x88
    volatile unsigned int  UCR4;          // UART Control Register 4, offset: 0x8C
    volatile unsigned int  UFCR;          // UART FIFO Control Register, offset: 0x90
    volatile unsigned int  USR1;          // UART Status Register 1, offset: 0x94
    volatile unsigned int  USR2;          // UART Status Register 2, offset: 0x98
    volatile unsigned int  UESC;          // UART Escape Character Register, offset: 0x9C
    volatile unsigned int  UTIM;          // UART Escape Timer Register, offset: 0xA0
    volatile unsigned int  UBIR;          // UART BRM Incremental Register, offset: 0xA4
    volatile unsigned int  UBMR;          // UART BRM Modulator Register, offset: 0xA8
    volatile unsigned int  UBRC;          // UART Baud Rate Count Register, offset: 0xAC
    volatile unsigned int  ONEMS;         // UART One Millisecond Register, offset: 0xB0
    volatile unsigned int  UTS;           // UART Test Register, offset: 0xB4
    volatile unsigned int  UMCR;          // UART RS-485 Mode Control Register, offset: 0xB8
} UART_Type;

// Pointer to UART1 registers.
#define UART1    ((UART_Type *)UART1_BASE)

// Function to initialize the UART.
void Uart_Init(void);

// Function to send a single character over UART.
void PutChar(int c);

// Function to receive a single character over UART.
unsigned char GetChar(void);

// Function to send a string over UART.
void PutStr(const char *s);

#endif /* _UART_H_ */
