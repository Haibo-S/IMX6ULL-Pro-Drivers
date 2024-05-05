#include "spi.h"
#include "my_printf.h"

/* SPI3 IOMUX base addresses */
#define UART2_TX_BASE    0x20e0094
#define UART2_RX_BASE    0x20e0098
#define UART2_RTS_BASE   0x20e009C
#define UART2_CTS_BASE   0x20e00A0

static volatile unsigned int *GPIO1_GDIR;
static volatile unsigned int *GPIO1_DR;

// Configure IOMUX for specific pin
static void iomuxc_sw_set(unsigned int base, unsigned int uc_mode)
{
    *((volatile unsigned int *)base) = uc_mode & 0x000f;
}

// Initialize the SPI controller
unsigned char spi_init(SPI_CTRL *uc_num)
{
    /* Reset the SPI module */
    uc_num->CONREG = 0;
    /* Configure CONREG: 8-bit burst, master mode, enable module */
    uc_num->CONREG |= (7 << 20) | (1 << 4) | (1 << 3) | (1 << 0);
    /* Default CONFIGREG settings */
    uc_num->CONFIGREG = 0;

    /* Configure SPI clock to 4MHz */
    uc_num->CONREG &= ~(0xf << 12 | 0xf << 8);
    uc_num->CONREG |= (14 << 12);

    /* Initialize GPIO pins */
    iomuxc_sw_set(UART2_TX_BASE, 5); /* Set as GPIO for chip select */
    GPIO1_GDIR = (volatile unsigned int *)(0x209C000 + 0x4);
    GPIO1_DR = (volatile unsigned int *)(0x209C000);
    *GPIO1_GDIR |= (1 << 20);
    *GPIO1_DR |= (1 << 20);

    iomuxc_sw_set(UART2_RX_BASE, 8);
    iomuxc_sw_set(UART2_RTS_BASE, 8);
    iomuxc_sw_set(UART2_CTS_BASE, 8);

    printf("SPI initialized successfully.\n\r");
    return 0;
}

// Assert the SPI chip select line
void spi_select(void)
{
    *GPIO1_DR &= ~(1 << 20);
}

// spi_deselect
void spi_deselect(void)
{
    *GPIO1_DR |= (1 << 20);
}

// Write and read data using SPI
unsigned char spi_writeread(SPI_CTRL *spi_num, unsigned char uc_txdata)
{
    /* Select channel 0 */
    spi_num->CONREG &= ~(3 << 18);
    spi_num->CONREG |= 0 << 18;

    /* Wait for FIFO to be empty */
    while (!(spi_num->STATREG & (1 << 0)));

    /* Transmit data */
    spi_num->TXDATA = uc_txdata;

    /* Wait for data to be received */
    while (!(spi_num->STATREG & (1 << 3)));

    return spi_num->RXDATA;
}

// Perform loopback test for SPI
unsigned char spi_test_rw(SPI_CTRL *spi_num)
{
    unsigned char uc_cnt;
    unsigned char uc_buf_write[20] = {0};
    unsigned char uc_buf_read[20] = {0};

    /* Enable loopback mode */
    spi_num->TESTREG = (1 << 31);
    printf("Entering SPI loopback test mode.\n\r");

    /* Prepare test data */
    for (uc_cnt = 0; uc_cnt < 20; uc_cnt++) {
        uc_buf_write[uc_cnt] = 0x20 + uc_cnt;
    }

    /* Perform read/write test */
    for (uc_cnt = 0; uc_cnt < 20; uc_cnt++) {
        printf("Write count: %d\t", uc_cnt);
        uc_buf_read[uc_cnt] = spi_writeread(spi_num, uc_buf_write[uc_cnt]);
        printf("Write: %d\t", uc_buf_write[uc_cnt]);
        printf("Read: %d\n\r", uc_buf_read[uc_cnt]);
    }

    /* Validate test results */
    for (uc_cnt = 0; uc_cnt < 20; uc_cnt++) {
        if (uc_buf_read[uc_cnt] != uc_buf_write[uc_cnt]) {
            printf("SPI loopback test failed!\n\r");
            return -1;
        }
    }

    printf("SPI loopback test passed.\n\r");
    printf("Exiting SPI loopback test mode.\n\r");

    /* Exit loopback mode */
    spi_num->TESTREG = 0;
    return 0;
}
