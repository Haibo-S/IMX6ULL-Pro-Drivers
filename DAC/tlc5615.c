#include "spi.h"
#include "tlc5615.h"
#include "my_printf.h"

void tlc5615_write_addr(unsigned short uc_data) {
	spi_writeread(ESCPI1_BASE,uc_data);
}		  			 		  						  					  				 	   		  	  	 	  

unsigned char tlc5615_init(void)
{
	unsigned char uc_dev_id = 0;
	spi_init(ESCPI1_BASE);
	printf("tlc5615 initalization starts\n\r");
	tlc5615_write_addr(0x00);
	printf("tlc5615 initalization ends\n\r");
	return 0;
}
		  			 		  						  					  				 	   		  	  	 	  
