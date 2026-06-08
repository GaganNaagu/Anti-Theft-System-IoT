# Smart Vehicle Monitoring and Anti-Theft System

## What This Is

This project is an embedded low-cost anti-theft system that monitors a vehicle for suspicious activities. It detects physical tampering (shaking/vibration) and lifting (towing/tilt) and triggers real-time visual and audio alerts (on an LCD screen, LED, and buzzer) and prints events to the serial monitor.

## Core Value

The system must reliably detect vehicle vibration or tilting and instantly trigger local audio and visual alarms.

## Requirements

### Validated

- ✓ SPI hardware interface configured for Waveshare 1.3-inch LCD (240x240, SPI) — existing
- ✓ Off-screen buffer drawing engine (`Paint_`) supporting lines, circles, boxes, text, and float numbers — existing
- ✓ Bitmap font assets (size 8, 12, 16, 20, 24 English and 24 Chinese) stored in program flash memory — existing

### Active

- [ ] Configure GPIO pins for sensors and alarm outputs: Vibration (SW-420) -> D2, Tilt -> D3 (INPUT_PULLUP), Buzzer -> D6, LED -> D5.
- [ ] Initialize system outputs and show "System Ready" splash on the 1.3-inch LCD.
- [ ] Monitor sensors in the main execution loop.
- [ ] Trigger buzzer and LED and display "ALERT! Movement Detected" on the 1.3-inch LCD when vibration (HIGH) or tilt (LOW) is detected.
- [ ] Log alert events to the serial console at 9600 baud rate.
- [ ] Revert to quiet state and display "Vehicle Safe" on the 1.3-inch LCD when no threats are detected.

### Out of Scope

- Use of SSD1306 I2C 128x64 OLED — Excluded because we are reusing the existing Waveshare 1.3-inch SPI LCD hardware.
- Remote cellular notifications (GSM SMS alerts) — Deferred to v2 future scope.
- Wi-Fi/Cloud dashboard integration (ESP32) — Deferred to v2 future scope.

## Context

- **Hardware Platform**: Arduino AVR compatible microcontroller (such as Uno or Nano).
- **Display**: Waveshare 1.3" SPI IPS LCD (ST7789 controller, 240x240 resolution).
- **Sensors**: SW-420 Vibration Sensor module, SW-520D (or similar rolling ball) Tilt Switch module.
- **Alarm Modules**: Active Piezo Buzzer and a 5mm LED with a 220Ω protective resistor.
- **Porting**: Codebase originally targeted a simple demo on the Waveshare LCD, now being refactored to implement the security monitor application described in the provided guide.

## Constraints

- **Microcontroller SRAM Limit**: Arduino Uno has only 2KB of SRAM. We must avoid allocating large full-screen buffers in micro-controller RAM and instead write pixels directly to the LCD's display memory where possible, and avoid dynamic memory allocations (`malloc`) to prevent heap fragmentation.
- **Microcontroller Pin Budget**: SPI display utilizes pins 7, 8, 9, 10, 11, 13. Sensor and alarm outputs must be assigned to the remaining free pins (D2, D3, D5, D6).

## Key Decisions

| Decision | Rationale | Outcome |
|----------|-----------|---------|
| Reuse 1.3" SPI LCD | Already configured in the codebase and available in user's physical components. | — Pending |
| Pin mappings: Vibration->D2, Tilt->D3, Buzzer->D6, LED->D5 | Avoids conflicts with LCD SPI pins (7-13) and leaves serial RX/TX pins (D0/D1) free. | — Pending |

## Evolution

This document evolves at phase transitions and milestone boundaries.

**After each phase transition** (via `/gsd-transition`):
1. Requirements invalidated? → Move to Out of Scope with reason
2. Requirements validated? → Move to Validated with phase reference
3. New requirements emerged? → Add to Active
4. Decisions to log? → Add to Key Decisions
5. "What This Is" still accurate? → Update if drifted

**After each milestone** (via `/gsd-complete-milestone`):
1. Full review of all sections
2. Core Value check — still the right priority?
3. Audit Out of Scope — reasons still valid?
4. Update Context with current state

---
*Last updated: 2026-06-08 after initialization*
