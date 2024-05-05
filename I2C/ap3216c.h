#ifndef __AP3216C_H
#define __AP3216C_H
#include "type.h"
#define AP3216C_ADDR    	0X1E	

/*
 * AP3216C操作码定义
 */
typedef enum enAP3216C_OPCODE
{
    AP3216C_SYSTEMCONFIG = 0,          
    AP3216C_INTSTATUS  = 1,  		   
    AP3216C_INTCLEAR   = 2,	           
    AP3216C_START_ALL  = 3,            
    AP3216C_INIT       = 4,            
    AP3216C_IRDATALOW	= 10,	       
    AP3216C_IRDATAHIGH	= 11,	       
    AP3216C_ALSDATALOW	= 12,	      
    AP3216C_ALSDATAHIGH	= 13,	       
    AP3216C_PSDATALOW	= 14,	       
    AP3216C_PSDATAHIGH	= 15,	       
    AP3216C_DONOTHING_BULL
} AP3216C_OPCODE;

unsigned char ap3216c_init(void);

void ap3216c_read_ir(uint16_t *ir);

void ap3216c_read_als(uint16_t *als);

void ap3216c_read_ps(uint16_t *ps);

void ap3216c_read_data(uint16_t *ir, uint16_t *ps, uint16_t *als);

#endif


