#include "spi.h"
#include "my_printf.h"

#define CSI_DATA7_BASE	0x20e0200 //
#define CSI_DATA6_BASE	0x20e01fc
#define CSI_DATA5_BASE	0x20e01f8
#define CSI_DATA4_BASE	0x20e01f4
#define CSI_DATA3_BASE	0x20e01f0

static void iomuxc_sw_set(unsigned int base,unsigned int uc_mode)
{
	*((volatile unsigned int *)base)  = uc_mode&0x000f;
}

unsigned char spi_init(SPI_CTRL *uc_num)
{
    printf("Initializing SPI...\n\r");

    /* Reset the SPI module */
    uc_num->CONREG = 0;

    /* Configure CONREG: 12-bit burst, master mode, enable module */
    uc_num->CONREG |= (11 << 20) | (1 << 4) | (1 << 3) | (1 << 0);

    /* Default CONFIGREG settings */
    uc_num->CONFIGREG = 0;

    /* Configure SPI clock to 0.5MHz */
    uc_num->CONREG &= ~(0xf << 12 | 0xf << 8);
    uc_num->CONREG |= (14 << 12 | 3 << 8);

    printf("SPI initialized.\n\r");

    /* Initialize pins */
    iomuxc_sw_set(CSI_DATA5_BASE, 3);
    iomuxc_sw_set(CSI_DATA4_BASE, 3);
    iomuxc_sw_set(CSI_DATA6_BASE, 3);
    iomuxc_sw_set(CSI_DATA7_BASE, 3);

    return 0;
}

// Write and read data using SPI
unsigned int spi_writeread(SPI_CTRL *spi_num, unsigned short uc_txdata)
{
    /* Select channel 0 */
    spi_num->CONREG &= ~(3 << 18);
    spi_num->CONREG |= 0 << 18;

    /* Wait for FIFO to be empty */
    while (!(spi_num->STATREG & (1 << 0)));

    /* Transmit data */
    spi_num->TXDATA = uc_txdata;

    /* Wait for data to be received */
    while (!(spi_num->STATREG & (1 << 7)));

    return spi_num->RXDATA;
}