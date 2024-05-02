#include "uart.h"

int main(){
	unsigned char cTestData = 'A';
 	Uart_Init();

	while(1){	
		PutChar(cTestData);
	}
					
	return 0;
}


