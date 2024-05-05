#include "spi.h"
#include "icm20608g.h"
#include "my_printf.h"
static ICM20608G_GYRO_ACC_adc icm20608g_get;

void icm20608g_write_addr(unsigned char uc_addr,unsigned char uc_data) {
	unsigned char uc_read=0;
	uc_addr &= ~0x80;
	spi_select();
	spi_writeread(ESCPI3_BASE,uc_addr);
	spi_writeread(ESCPI3_BASE,uc_data);
	spi_deselect();
}		  			 		  						  					  				 	   		  	  	 	  

unsigned char  icm20608g_read_addr(unsigned char uc_addr) {
	unsigned char uc_read = 0;
	uc_addr |= 0x80;
	spi_select();
	spi_writeread(ESCPI3_BASE,uc_addr);
	uc_read=spi_writeread(ESCPI3_BASE,0xff);
	spi_deselect();
	return uc_read;
}

unsigned char icm20608g_init(void)
{
	unsigned char uc_dev_id = 0;
	spi_init(ESCPI3_BASE);
	printf("icm20608g initalization starts\n\r");
//	icm20608_write_addr(ICM20608G_PWR_MGMT_1,0x80);
	icm20608g_write_addr(ICM20608G_PWR_MGMT_1,0x01);
	uc_dev_id = icm20608g_read_addr(ICM20608G_WHO_AM_I);
	printf("read icm20608g id is 0x%x\n\r",uc_dev_id);
	if(uc_dev_id!=0xAF)
	{
		printf("read id fail\n\r");
		return -1;
	}
	icm20608g_write_addr(ICM20608G_SMPLRT_DIV,0x00);
	icm20608g_write_addr(ICM20608G_CONFIG, 0x00);
	icm20608g_write_addr(ICM20608G_GYRO_CONFIG,0x00);
	icm20608g_write_addr(ICM20608G_ACC_CONFIG,0x00);
	icm20608g_write_addr(ICM20608G_ACC_CONFIG2,0x00);
	icm20608g_write_addr(ICM20608G_LP_MODE_CFG,0x00);
	icm20608g_write_addr(ICM20608G_FIFO_EN,0x00);
	icm20608g_write_addr(ICM20608G_PWR_MGMT_2,0x00);
	printf("icm20608g initialization ends\n\r");
	return 0;
}

unsigned char icm20608g_read_len(unsigned char uc_addr,unsigned char *buf,unsigned char uc_len) {
	unsigned char uc_cnt;
	uc_addr |= 0x80;
	spi_select();
	spi_writeread(ESCPI3_BASE,uc_addr);
	for(uc_cnt=0;uc_cnt<uc_len;uc_cnt++)
	{
		buf[uc_cnt]=spi_writeread(ESCPI3_BASE,0xff);
	}
	spi_deselect();
	return 0;
}

void print_x(unsigned char *uc_buf,unsigned char uc_len) {
	unsigned char uc_cnt;
	for(uc_cnt=0;uc_cnt<uc_len;uc_cnt++)
	{
		printf("read %d : %x \n\r",uc_cnt,uc_buf[uc_cnt]);
	}	
}

unsigned char icm20608g_read_acc(void) {
	unsigned char uc_buf[6];
	icm20608g_read_len(0x3b,uc_buf,6);
	icm20608g_get.acc_x_adc = (signed short)((uc_buf[0]<<8)|uc_buf[1]);
	icm20608g_get.acc_y_adc = (signed short)((uc_buf[2]<<8)|uc_buf[3]);
	icm20608g_get.acc_z_adc = (signed short)((uc_buf[4]<<8)|uc_buf[5]);
	printf("icm20608g_read_acc \n\r");
	print_x(uc_buf,6);
	return 0;
}

unsigned char  icm20608g_read_gyro(void) {
	unsigned char uc_buf[6];
	icm20608g_read_len(0x43,uc_buf,6);
	icm20608g_get.gyro_x_adc = (signed short)((uc_buf[0]<<8)|uc_buf[1]);
	icm20608g_get.gyro_y_adc = (signed short)((uc_buf[2]<<8)|uc_buf[3]);
	icm20608g_get.gyro_z_adc = (signed short)((uc_buf[4]<<8)|uc_buf[5]);
	printf("icm20608g_read_gyro \n\r");
	print_x(uc_buf,6);
	return 0;
}
		  			 		  						  					  				 	   		  	  	 	  

unsigned char icm20608g_read_temp(void) {
	unsigned char uc_buf[2];
	icm20608g_read_len(0x41,uc_buf,2);
	icm20608g_get.temp_adc = (signed short)((uc_buf[0]<<8)|uc_buf[1]);
	printf("icm20608g_read_temp \n\r");
	print_x(uc_buf,2);
	return 0;
}




