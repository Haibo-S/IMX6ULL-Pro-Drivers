## IMX6ULL Pro Development Board Drivers

This repository provides drivers and example code for various components and peripherals of the i.MX 6ULL Pro development board.

## Overview

The i.MX 6ULL Pro development board is an integrated single-chip solution suitable for various applications. This repository provides low-level drivers for the board's peripherals, including LED, Clock, Exception and Interrupt, DAC, SPI, I2C, UART, and LCD. 


## Development Environment

To build and deploy the drivers for the i.MX 6ULL Pro development board, set up a suitable development environment with the following tools:

### About the Chip

The i.MX 6ULL is a low-power application processor from NXP, based on the ARM Cortex-A7 core.

### Toolchain

The drivers can be compiled using the GNU Arm Embedded Toolchain. Install it by following these steps:

```bash
sudo apt-get update
sudo apt-get install gcc-arm-none-eabi
```
- **IDE**: ARM DS-5, Keil, or Eclipse.
- **Compiler**: ARM GCC or Keil ARMCC.
- **Debugger**: JTAG or SWD compatible with ARM Cortex-M processors.
- **Operating** System: Linux

### Build System
The repository uses a Makefile-based build system. To build the project, you need make installed:

``` bash
sudo apt-get install make
```

### Emulator/Debugger
To debug the drivers, you can use OpenOCD and GDB:

``` bash
sudo apt-get install openocd gdb-multiarch
```

### Setup 
1. **Clone the Repository:**

``` bash
git clone git@github.com:Haibo-S/IMX6ULL-Pro-Drivers.git
```

2. **Navigate into the repo:**

``` bash
cd IMX6ULL-Pro-Drivers
```

3. **Build**

``` bash
make
```

4. **Upload to Board**

Use OpenOCD and GDB to upload and debug the code on the i.MX 6ULL Pro development board.

### Quick Start:
For a quick start, try the following example code, which toggles an onboard LED:

``` c 
#include "led.h"

int main(void) {
    led_init();
    while (1) {
        led_on(1);
        delay(500);
        led_off(1);
        delay(500);
    }
    return 0;
}

```


## Hardware Overview

The i.MX 6ULL Pro development board is built around the NXP i.MX 6ULL system-on-chip (SoC), which features a single ARM Cortex-A7 core running at up to 528 MHz. The hardware specifications and supported peripherals include:

1. **Processor**:
   - **Core**: ARM Cortex-A7
   - **Frequency**: Up to 528 MHz
   - **Architecture**: 32-bit RISC

2. **Memory**:
   - **RAM**: 512 MB to 1 GB of DDR3 or DDR3L
   - **Flash Storage**: 512 MB to 1 GB of NAND or eMMC

3. **Networking**:
   - **Ethernet**: 10/100 Mbps
   - **Wi-Fi**: Optional module
   - **Bluetooth**: Optional module

4. **Display**:
   - **LCD Controller**: Supports TFT panels up to 1366 x 768 resolution

5. **Audio**:
   - **DAC**: 16-bit resolution

6. **Connectivity**:
   - **UART**: Up to 5 ports
   - **SPI**: Up to 2 ports
   - **I2C**: Up to 4 ports

7. **GPIO**:
   - **Pins**: Up to 128 programmable I/O pins

8. **Other Peripherals**:
   - **ADC**: 10-bit resolution
   - **PWM**: Up to 4 channels

