# Technology Stack

**Analysis Date:** 2026-06-08

## Languages

**Primary:**
- C++ (Arduino Dialect) - All application logic, driver code, and GUI drawing routines.

**Secondary:**
- None.

## Runtime

**Environment:**
- Arduino AVR Runtime - Designed for AVR-microcontrollers (e.g., ATmega328P on Arduino Uno/Nano).
- Uses AVR-specific features like `avr/pgmspace.h` for storing fonts and images in program memory (Flash) instead of SRAM.

**Package Manager:**
- None (manual file inclusion).

## Frameworks

**Core:**
- Arduino Core SDK - Standard Arduino setup/loop execution model and standard GPIO/SPI APIs.

**Testing:**
- None.

**Build/Dev:**
- Arduino IDE / CLI.

## Key Dependencies

**Critical:**
- `SPI.h` (Built-in Arduino library) - Provides SPI communication interface to the LCD hardware driver.

**Infrastructure:**
- None.

## Configuration

**Environment:**
- Hardcoded pin configurations in `DEV_Config.h`.

**Build:**
- Compile parameters are determined by the Arduino IDE board selection (AVR).

## Platform Requirements

**Development:**
- Arduino IDE or compatible build system.

**Production:**
- AVR Microcontroller (such as Arduino Uno, Nano, or similar) connected to a Waveshare 1.3inch LCD display module via SPI.

---

*Stack analysis: 2026-06-08*
