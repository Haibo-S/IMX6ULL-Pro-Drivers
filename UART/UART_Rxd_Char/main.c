#include "uart.h"

int main(){
	unsigned char cTestData;       
 	Uart_Init();

	while(1){	
		cTestData = GetChar();				
		PutChar(cTestData);				
	}
					
	return 0;
}


