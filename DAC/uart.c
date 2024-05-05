#include "uart.h"

static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA ;
static volatile unsigned int *IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA	;

void Uart_Init(void) {
	
	IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA		= (volatile unsigned int *)(0x20E0084);
	IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA		= (volatile unsigned int *)(0x20E0088);

	*IOMUXC_SW_MUX_CTL_PAD_UART1_RX_DATA = 0;
	*IOMUXC_SW_MUX_CTL_PAD_UART1_TX_DATA = 0;

	UART1->UCR1 |= (1 << 0) ;		
	
	UART1->UCR2 |= (1<<14) |(1<<5) |(1<<2)|(1<<1);

	UART1->UCR3 |= (1<<2);
	
    UART1->UFCR = 5 << 7;       
    UART1->UBIR = 71;
    UART1->UBMR = 3124;

	UART1->UCR1 |= (1 << 0);		
}

void PutChar(int c) {
	while (!((UART1->USR2) & (1<<3))); 
	UART1->UTXD = (unsigned char)c;		
}

unsigned char GetChar(void)						
{	
	while (!(UART1->USR2 & (1<<0)));  
	return (unsigned char)UART1->URXD;
}

void PutStr(const char *s)				
{
	while (*s)
	{
		PutChar(*s);
		s++;
	}
}

int raise(int signum)
{
    return 0;
}


