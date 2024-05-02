#include "uart.h"

int  main(){
	unsigned char cTestData ;       
 	Uart_Init()	 ;

	PutStr("Hello, world!\n\r");	
	
	while(1){	
		cTestData = GetChar();				
		
		if (cTestData == '\r') { 
			PutChar('\n');
		}

		if (cTestData == '\n') {
			PutChar('\r');
		}
				
		PutChar(cTestData);				
	}
					
	return 0;
}
		  			 		  						  					  				 	   		  	  	 	  
