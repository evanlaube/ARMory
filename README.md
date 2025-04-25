
# ARMory
A bare-metal hardware abstraction layer for STM32 microcontrollers

## 🧰 What is ARMory?
ARMory is a hardware abstraction layer (HAL) designed for use with STM32 based
microcontrollers. Its goal is to create a fast, efficient way to easily control
devices using these microcontrollers. ARMory is written entirely in C.

## 🔌 Supported hardware

- **STM32F411**

Currently, the only board supported by ARMory is the STM32F411. However, while
support for more boards is not set in stone for the future, the library is
planned to be written in a way that allows other boards to be easily supported
further down the road. Until then, an entire set of features will be created 
for the STM32F411, an ARM Cortex-M4 microcontroller.

## ⚙️ Current Features

- **GPIO**
    - Set pin modes (input, output, analog, alternate)
    - Read and write digital pins 
    - Set pull-up, pull-down, or no-pull configurations
    - Supports GPIO ports A, B, and C

- **PWM**
    - Output PWM on supported timer channels
    - Configure duty cycle of pins using using 8 bits (0-255)

- **I<sup>2</sup>C**
    - Master-mode communication
    - Start/Stop Signals, and ACK/NACK handling
    - Write to device registers, or write raw bytes to devices

- **ADC Access**
    - Access to Analog to Digital converters on valid GPIO pins
    - Read analog values with 12-bit precision

- **Timing**
    - Very basic delay functions `delay_ms` and `delay_us`
    - Calibrated for 84 MHz system clock

- **Startup and Linker**
    - Minimal custom bootloader and vector table
    - Linker script tailored to STM32F411 memory map
    

## 🗃️ Project Structure
As ARMory is a HAL, or a hardware abstraction **library** the main project does
not do anything on its own. For this reason, a directory of examples is included,
which includes projects showing how the features of ARMory work. 

The project is organized into directories as follows
- `src/` - The core files of ARMory. This is where all library logic is written
- `include/` - Public header files (.h files)
- `examples/` - Directory of example folders for demonstrating features
- `makefile` - Compilation and example flashing procedures
- `linker.ld` - Linkerscript to set memory map - for STM32F411 only right now


## 🚀 Getting Started
The building process is streamlined in the makefile, where the project can be compiled and installed.

### 🧬 Cloning
The first step is cloning the repository:
```bash
git clone https://github.com/evanlaube/ARMory
cd ARMory 
```

### 📦 Installing Dependencies

As code will have to be compiled for ARM-based microcontrollers, the following
build dependencies must be installed.

#### ARM GCC Toolchain
Includes `arm-none-eabi-gcc`, `binutils`, and `newlib`

On Arch Linux:
``` bash
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-newlib
```

On macOS:
```bash
brew tap ArmMbed/homebrew-formulae
brew install arm-none-eabi-gcc
```

#### ST-Flash
Optional, but allows for code, like the examples in the repo, to be flashed to 
the board. Other programs can be used to flash, but I use ST-Flash, and it's in
the makefile.

On Arch Linux:
```bash
sudo pacman -S stlink
```

On macOS:
```bash
brew install stlink
```

> **Note:** These are the install commands for the OSes that I use, but the tools can be downloaded on any OS!

### 🧱 Compilation and Installation

#### 🛠️ Compiling

To simply compile the library, run:
```bash
make
```

#### 🗂️ Installing

This places a `libarmory.a` file in the `build/` directory in the project folder.
However, to **install** the library to the system for easier portability and usage
in other projects, run:
```bash
sudo make install
```
This will install the static library to `/usr/local/lib/libarmory.a`, and install
the header files to `/usr/local/include/armory`. 
> **Note:** These install targets in the makefile only work for Linux and macos.
While the library can be used with Windows, it is not supported in the makefile,
and has to be manually installed.

#### ❌ Uninstall
Uninstalling is simple! Simply run:
```bash
sudo make uninstall
```

#### 🔦 Flashing an Example
Examples in the `examples/` directory can be easily flashed to an STM32F411 board.
Simply run:

```bash
make flash EXAMPLE=[your_example]
```

Replacing the entire `[your_example]` with the name of one if the sub-directories
in the `examples` directory. This will build that example and flash it to the board.

> Check the corresponding example's source code for wiring details


## 🤝 How to Contribute

Contributions are welcome! If you have ideas for new features, bug fixes, or 
improvements, feel free to [open an issue](https://github.com/evanlaube/ARMory/issues)
or submit a pull request.
Please read the [CONTRIBUTING.md](CONTRIBUTING.md) file for detailed guidelines
on how to contribute.


## 📄 License Notice

Armory is licensed under the MIT License.
You are free to use, modify, and distribute this software as long as the 
original license is included.

See the [LICENSE](LICENSE) file for full details.

## 📣 Get Involved 
If you have any questions, suggestions, or feedback, please don't hesitate to
[open an issue](https://github.com/evanlaube/ARMory/issues) or 
[contact me](mailto:laubeevan@gmail.com). Feedback of all kinds is welcome! 

---

 >🪵 You've scolled too far, and made it to `_sstack` -
 be careful, anything past this is undefined behavior

