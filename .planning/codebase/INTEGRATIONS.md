# External Integrations

**Analysis Date:** 2026-06-08

## APIs & External Services

None. The system operates fully offline as a standalone micro-controller application.

## Data Storage

**Databases:**
- None.

**File Storage:**
- Flash Memory (Program Space) - Fonts (`font*.cpp`) and image assets (`image.cpp`) are stored as static arrays in program memory using `PROGMEM` via `<avr/pgmspace.h>` to fit within the limited SRAM of AVR chips.

## Authentication & Identity

None.

## Monitoring & Observability

**Logs:**
- Serial output - Conditional debugging logs printed via standard Arduino `Serial` depending on `DEBUG` settings in `Debug.h`.

## CI/CD & Deployment

None. Deployment is performed manually via Arduino IDE/CLI flashing over USB (serial interface).

## Environment Configuration

**Development:**
- Physical wiring of pins (defined in `DEV_Config.h`) must match the actual hardware connections between the microcontroller and the LCD panel:
  - VCC -> 5V/3.3V
  - GND -> GND
  - DIN (MOSI) -> Pin 11 (Arduino Hardware SPI MOSI)
  - CLK (SCK) -> Pin 13 (Arduino Hardware SPI SCK)
  - CS -> Pin 10 (Chip Select)
  - DC -> Pin 7 (Data/Command Select)
  - RST -> Pin 8 (Reset)
  - BL -> Pin 9 (Backlight PWM)

## Webhooks & Callbacks

None.

---

*Integration audit: 2026-06-08*
