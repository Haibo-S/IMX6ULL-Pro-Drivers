#include "i2c.h"
void i2c_init(I2C_REGISTERS *I2C_BASE) {

	I2C_BASE->I2CR &= ~(1 << 7);
	I2C_BASE->IFDR = 0x37;
	I2C_BASE->I2CR |= (1<<7);
}

uint8_t i2c_check(I2C_REGISTERS *I2C_BASE, uint32_t status) {
	if(status & (1<<4)) {
		I2C_BASE->I2SR &= ~(1<<4);	

		I2C_BASE->I2CR &= ~(1 << 7);	
		I2C_BASE->I2CR |= (1 << 7);	
		return I2C_ARBITRATIONLOST;
	} else if(status & (1 << 0)) {
		return I2C_NAK;		
	}
	return I2C_OK;

}

uint8_t i2c_start(I2C_REGISTERS *I2C_BASE, uint8_t ucSlaveAddr, uint32_t ulOpcode) {

	if(I2C_BASE->I2SR & (1 << 5))			
		return 1;

	I2C_BASE->I2CR |=  (1 << 5) | (1 << 4);

	I2C_BASE->I2DR = ((uint32_t)ucSlaveAddr << 1) | ((I2C_READ == ulOpcode)? 1 : 0);
	return 0;

}

uint8_t i2c_stop(I2C_REGISTERS *I2C_BASE) {

	uint16_t usTimeout = 0xffff;

	I2C_BASE->I2CR &= ~((1 << 5) | (1 << 4) | (1 << 3));

	while((I2C_BASE->I2SR & (1 << 5))) {
		usTimeout--;
		if(usTimeout == 0)	
			return I2C_TIMEOUT;
	}
	return I2C_OK;

}

uint8_t i2c_restart(I2C_REGISTERS *I2C_BASE, uint8_t ucSlaveAddr, uint32_t ulOpcode) {

	if(I2C_BASE->I2SR & (1 << 5) && (((I2C_BASE->I2CR) & (1 << 5)) == 0))		
		return 6;

	I2C_BASE->I2CR |=  (1 << 4) | (1 << 2);

	I2C_BASE->I2DR = ((uint32_t)ucSlaveAddr << 1) | ((I2C_READ == ulOpcode)? 1 : 0);
	
	return 0;

}


void i2c_write(I2C_REGISTERS *I2C_BASE, const uint8_t *pbuf, uint32_t len) {
	while(!(I2C_BASE->I2SR & (1 << 7))); 
	
	I2C_BASE->I2SR &= ~(1 << 1); 
	I2C_BASE->I2CR |= 1 << 4;	     
	while(len--)
	{
		I2C_BASE->I2DR = *pbuf++; 	    
		
		while(!(I2C_BASE->I2SR & (1 << 1))); 
		I2C_BASE->I2SR &= ~(1 << 1);			

		if(i2c_check(I2C_BASE, I2C_BASE->I2SR))
			break;
	}
	
	I2C_BASE->I2SR &= ~(1 << 1);    
	i2c_stop(I2C_BASE); 	        

}

void i2c_read(I2C_REGISTERS *I2C_BASE, uint8_t *pbuf, uint32_t len) {
	volatile uint8_t dummy = 0;
	dummy++; 	

	while(!(I2C_BASE->I2SR & (1 << 7))); 
	
	I2C_BASE->I2SR &= ~(1 << 1); 			  
	I2C_BASE->I2CR &= ~((1 << 4) | (1 << 3));	
	
	if(len == 1)
        I2C_BASE->I2CR |= (1 << 3);

	dummy = I2C_BASE->I2DR; 


	while(len--) {
		while(!(I2C_BASE->I2SR & (1 << 1))); 
		I2C_BASE->I2SR &= ~(1 << 1);			
	 	if(len == 0)
        {
        	i2c_stop(I2C_BASE); 			
        }

        if(len == 1)
        {
            I2C_BASE->I2CR |= (1 << 3);
        }
		*pbuf++ = I2C_BASE->I2DR;
	}

}

uint8_t i2c_transfer(I2C_REGISTERS *I2C_BASE, I2C_TRANSFER *transfer) {
	uint32_t ulRet = 0;
	uint32_t ulOpcode = transfer->ulOpcode;

	I2C_BASE->I2SR &= ~((1 << 1) | (1 << 4));
	while(!((I2C_BASE->I2SR >> 7) & 0X1)){}; 

    if ((transfer->ulSubAddressLen > 0) && (transfer->ulOpcode == I2C_READ))
    {
        ulOpcode = I2C_WRITE;
    }
	ulRet = i2c_start(I2C_BASE, transfer->ucSlaveAddress, ulOpcode);

	if (ulRet)
	{
		return ulRet;
	}
	
	while(!(I2C_BASE->I2SR & (1 << 1))){};

	ulRet = i2c_check(I2C_BASE, I2C_BASE->I2SR);

	if (ulRet) {
	    i2c_stop(I2C_BASE); 			
		return ulRet;
	}

	if (transfer->ulSubAddressLen) {
		do {
		    I2C_BASE->I2SR &= ~(1 << 1); 
			
			transfer->ulSubAddressLen--;

			I2C_BASE->I2DR = ((transfer->ulSubAddress) >> (8 * transfer->ulSubAddressLen)); 
  
			while(!(I2C_BASE->I2SR & (1 << 1)));

            ulRet = i2c_check(I2C_BASE, I2C_BASE->I2SR);
            if(ulRet)
            {
             	i2c_stop(I2C_BASE); 				
             	return ulRet;
            }
		}
		while ((transfer->ulSubAddressLen > 0) && (ulRet == I2C_OK));

		if (I2C_READ == transfer->ulOpcode)
		{
            I2C_BASE->I2SR &= ~(1 << 1);
            i2c_restart(I2C_BASE, transfer->ucSlaveAddress, I2C_READ); 
    		while(!(I2C_BASE->I2SR & (1 << 1))){}; 

			ulRet = i2c_check(I2C_BASE, I2C_BASE->I2SR);
			
            if(ulRet)
            {
             	ulRet = I2C_ADDRNAK;
                i2c_stop(I2C_BASE); 		
                return ulRet;  
            }
           	       

		}
		
	}
    if ((I2C_WRITE == transfer->ulOpcode) && (transfer->ulLenth > 0))
    {
    	i2c_write(I2C_BASE, transfer->pbuf, transfer->ulLenth);
	}

    if ((I2C_READ == transfer->ulOpcode) && (transfer->ulLenth > 0))
    {
       	i2c_read(I2C_BASE, transfer->pbuf, transfer->ulLenth);
	}
	return 0;	

}
uint8_t i2c_write_one_byte(uint8_t addr,uint8_t reg, uint8_t data,I2C_REGISTERS *I2C_BASE) {
    uint8_t status = 0;
    uint8_t writedata=data;
    I2C_TRANSFER transfer;
	
   	transfer.ucSlaveAddress = addr; 			
    transfer.ulOpcode = I2C_WRITE;			    
    transfer.ulSubAddress = reg;				
    transfer.ulSubAddressLen = 1;				
    transfer.pbuf = &writedata;				  
    transfer.ulLenth = 1;  					    

    status = i2c_transfer(I2C_BASE, &transfer);
    return status;
}

uint8_t i2c_read_one_byte(uint8_t addr, uint8_t reg,I2C_REGISTERS *I2C_BASE) {
	uint8_t val=0;
    uint8_t status = 0;	
    I2C_TRANSFER transfer;

	transfer.ucSlaveAddress = addr;				
    transfer.ulOpcode = I2C_READ;			    
    transfer.ulSubAddress = reg;				
    transfer.ulSubAddressLen = 1;				
    transfer.pbuf = &val;						
    transfer.ulLenth = 1;					    

    status = i2c_transfer(I2C_BASE, &transfer);
	return val;
}


