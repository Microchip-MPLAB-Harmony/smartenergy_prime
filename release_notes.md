![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip MPLAB® Harmony 3 Release Notes

## Harmony 3 Smart Energy PRIME v1.0.0

### New Features

- Added verification of signature in Firmware Upgrade service using ECDSA and SHA-256

### Bug fixes

- Tested PRIME 1.3.
- Tested PRIME 1.4 with frequency hopping.

### Known Issues

- Crypto_v4 Repository v4.0.0-E3 is not fully validated in this Engineer Release for PIC32CX MT platfrom. So ECDSA and SHA, used by the Firmware Upgrade service, are executed by Software instead of Hardware.

### Development Tools

- [MPLAB® X IDE v6.25](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.60](https://www.microchip.com/mplab/compilers)
- MPLAB® X IDE plug-ins:
  - MPLAB® Code Configurator 5.5.2 or higher
- PIC32CX-MT family (MCUs):
  - PIC32CX-MT DFP 1.3.132 or higher

### Notes

- None.

## Harmony 3 Smart Energy PRIME v1.0.0-E1

### New Features

Smart Energy PRIME repository contains the code that implements the PRIME Stack as defined in the Standard Specification from the [PRIME-Alliance](https://prime-alliance.org/)
Provided Modules are:
- PRIME libraries for the different versions and nodes in PRIME:
  - PRIME Base node 1.3
  - PRIME Base node 1.4
  - PRIME Service node 1.3
  - PRIME Service node 1.4
    All these libraries include MAC layer and convergence layers NULL and IEC-61334-4-32.
- Specific phy driver for PRIME (PHY Serial).
- Specific services for PRIME stack.
- PRIME PAL (Physical Abstraction Layers) for
  - PLC. Provides abstraction between MAC and Microchip implementation of PRIME PLC Driver.
  - RF. Provides abstraction between MAC and Microchip implementation of PRIME RF Driver.
  - Serial. Provides abstraction between MAC and the Microchip implementation of the PRIME PHY serial Driver.

### Known Issues

- PRIME 1.4 frequency hopping not tested.
- PRIME 1.4 Firwmare Upgrade cannot verify signature from a binary file.
- PRIME 1.3 Not tested.
- No documentation provided.