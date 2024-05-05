#ifndef _I2C_H
#define _I2C_H

#include "type.h"

/* I2C status codes for error handling */
#define I2C_OK                  (0)
#define I2C_ERROR               (1)
#define I2C_BUSY                (2)
#define I2C_IDLE                (3)
#define I2C_NAK                 (4)
#define I2C_ARBITRATIONLOST     (5)
#define I2C_TIMEOUT             (6)
#define I2C_ADDRNAK             (7)

/* I2C register addresses and configuration */
#define I2C1_BASE_ADDR          (0x21A0000u)
#define I2C2_BASE_ADDR          (0x21A4000u)
#define I2C1                    ((I2C_REGISTERS *)I2C1_BASE_ADDR)
#define I2C2                    ((I2C_REGISTERS *)I2C2_BASE_ADDR)

/* all registers address is Base address + xh offset*/
typedef struct tagRegisters{
  volatile uint16_t IADR;                              /*I2C Address Register, offset: 0x0 */
           uint8_t ReservedIADR[2];
  volatile uint16_t IFDR;                              /*I2C Frequency Divider Register, offset: 0x4 */
           uint8_t ReservedIFDR[2];
  volatile uint16_t I2CR;                              /*I2C Control Register, offset: 0x8 */
           uint8_t ReservedI2CR[2];
  volatile uint16_t I2SR;                              /*I2C Status Register, offset: 0xC */
           uint8_t ReservedI2SR[2];
  volatile uint16_t I2DR;                              /*I2C Data I/O Register, offset: 0x10 */
} I2C_REGISTERS;

typedef enum enI2C_OPCODE
{
    I2C_WRITE = 0,            
    I2C_READ  = 1,  	
    I2C_DONOTHING_BULL
} I2C_OPCODE;

typedef struct tagI2cTransfer
{
    uint8_t  ucSlaveAddress;      	     
    uint32_t ulOpcode  ; 		    
    uint32_t ulSubAddress;       		
    uint8_t  ulSubAddressLen;    	  
    volatile uint32_t ulLenth;  	   
    uint8_t *volatile pbuf;    	
} I2C_TRANSFER;


void i2c_init(I2C_REGISTERS *I2C_BASE);

uint8_t i2c_check(I2C_REGISTERS *I2C_BASE, uint32_t status);

uint8_t i2c_start(I2C_REGISTERS *I2C_BASE, uint8_t ucSlaveAddr, uint32_t ulOpcode);

uint8_t i2c_stop(I2C_REGISTERS *I2C_BASE);

uint8_t i2c_restart(I2C_REGISTERS *I2C_BASE, uint8_t ucSlaveAddr, uint32_t ulOpcode);

void i2c_write(I2C_REGISTERS *I2C_BASE, const uint8_t *pbuf, uint32_t len);

void i2c_read(I2C_REGISTERS *I2C_BASE, uint8_t *pbuf, uint32_t len);

uint8_t i2c_transfer(I2C_REGISTERS *I2C_BASE, I2C_TRANSFER *transfer);

uint8_t i2c_write_one_byte(uint8_t addr,uint8_t reg, uint8_t data, I2C_REGISTERS *I2C_BASE);

uint8_t i2c_read_one_byte(uint8_t addr, uint8_t reg, I2C_REGISTERS *I2C_BASE);

#endif
