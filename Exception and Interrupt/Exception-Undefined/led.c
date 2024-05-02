#include "led.h"

#if 0
static volatile unsigned int *CCM_CCGR1                              ;
static volatile unsigned int *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
static volatile unsigned int *GPIO5_GDIR                             ;
static volatile unsigned int *GPIO5_DR                               ;
#endif

void led_gpio_init(void) {
#if 0
	unsigned int val;
	
	CCM_CCGR1                               = (volatile unsigned int *)(0x20C406C);
	IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = (volatile unsigned int *)(0x2290014);
	GPIO5_GDIR                              = (volatile unsigned int *)(0x020AC000 + 0x4);
	GPIO5_DR                                = (volatile unsigned int *)(0x020AC000);

	*CCM_CCGR1 |= (3<<30);

	val = *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
	val &= ~(0xf);
	val |= (5);
	*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = val;
#endif	
	
	//*GPIO5_GDIR |= (1<<3);
	GPIO5->GDIR |= (1<<3);

}

void led_ctl(int on)
{
	if (on) {
		GPIO5->DR &= ~(1<<3);
	}
	else {
		GPIO5->DR |= (1<<3);
	}
}




	
	
