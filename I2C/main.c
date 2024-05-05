#include "i2c.h"
#include "type.h"
#include "ap3216c.h"
#include "led.h"
void delay(volatile unsigned int d)
{
	while(d--);
}

int  main()
{
	int ret = 0;
	uint16_t ir;
	uint16_t ps;
	uint16_t als;

	led_init();

	ret = ap3216c_init();
	if (ret)
	{	
		while(1)
		{
			led_ctl(1);
			delay(100000);
			led_ctl(0);
			delay(100000);
		}

	}
	else
	{
		led_ctl(1);
		delay(100000);
		led_ctl(0);
	}

	while(!ret)
	{	
		ap3216c_read_data(&ir,&ps,&als);
		if (als>100 || ps <200)
		{
			led_ctl(1);
		}
		else
		{
			led_ctl(0);
		}
	}
					
	return 0;
}


