# Bare Metal TI AM6254 Project
This repository implements a fully manually-written bare-metal project for the TI AM6254 (PocketBeagle2 Board) without using TI's SDK.

Features :

  -  Quad-core ARM Cortex-A53 booting support
  - MMU Setup for 4GB space 
  - GIC configuration
  - Interrupt vector tables for the Quad-core ARM Cortex-A53
  - One timer interrupt per core
  - LED blinking from all Quad-core ARM Cortex-A53

A clear and easy-to-understand implementation in C11 and assembly with a build system based on GNU Make makes this project both fun and educational.


This repository provides valuable insight into starting a bare-metal TI's AM6254 project.

## Secondary Bootloader (SBL)

This project is using a modified TI SD-SBL for the PocketBeagle2 board that boots raw images (no headers required) on all available cores of the SoC (M4F, R5F, and the quad-core A53).

The SBL makes A53-Core0 the master of the quad-core A53 cluster (the last one that jumps to application).

The SBL supports a `config.txt` file located at the root of the SD card. The file can contain some or all of the following parameters:

| Parameter           | Default Value (if not present in `config.txt`) | Description                                                  |
| ------------------- | ---------------------------------------------- | ------------------------------------------------------------ |
| `m4f_start_address` | `0x05000000`                                   | Specifies the start address of the M4F core. This address is hardcoded as `0x0000_0000` (mapped to `0x0500_0000` in the R5F's memory map) in the M4F's VTOR register. It must point to a valid interrupt vector table to correctly boot the core. If defined in `config.txt`, it **must** be `0x05000000`. |
| `r5f_start_address` | `0x70000000`                                   | Specifies the start address for the R5F core.                |
| `a53_start_address` | `0x80000000`                                   | Specifies the common start address for all A53 cores (they all boot from the same address). |
| `m4f_clock_in_Hz`   | `400000000`                                    | Specifies the clock frequency for the M4F core (in Hz).      |
| `r5f_clock_in_Hz`   | `400000000`                                    | Specifies the clock frequency for the R5F core (in Hz).      |
| `a53_clock_in_Hz`   | `1400000000`                                   | Specifies the clock frequency for the A53 cores (in Hz).     |
| `m4f_binary_file`   | `app_m4f.bin`                                  | Name of the M4F raw binary file to be loaded.                |
| `r5f_binary_file`   | `app_r5f.bin`                                  | Name of the R5F raw binary file to be loaded.                |
| `a53_binary_file`   | `app_a53.bin`                                  | Name of the A53 raw binary file to be loaded.                |

The file `tiboot3.bin` is the prebuilt SBL binary used to boot raw images on the PocketBeagle2 board.

## Details on the Application

The start-up code sets up the runtime environment for the quad-core ARM Cortex-A53 and configures the MMU with a flat mapping of the first 4 GB of memory, following the AM6254 memory map.

The application runs in EL3. It initializes the GIC and enables the timer interrupt for the quad-core ARM Cortex-A53. 

Four LEDs blink (one for each core) controlled from the timer ISR.

## Building the Application

To build the project, you need an installed ARM GCC compiler for 64-bit architecture (aarch64-none-elf).

Run the following commands :

```sh
cd ./Build
Rebuild.sh a53
```

The build process generates the following outputs in the `Output` directory :

  - ELF file
  - HEX mask
  - Assembly listing
  - MAP file
  - Binary file (must be copied to the root of the SD card)

## Continuous Integration

CI runs on pushes and pull-requests with a simple build and result verification on `ubuntu-latest` using GitHub Actions.
