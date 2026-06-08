# Research: Technology Stack (Anti-Theft System)

**Analysis Date:** 2026-06-08

## Overview
Subsequent research for integrating anti-theft sensors (SW-420, SW-520D) and alerts (Buzzer, LED) with the existing Waveshare 1.3" SPI LCD codebase.

## Sensor & Alert Stacks

**SW-420 Vibration Sensor:**
- Interface: Digital input pin.
- Logic: Outputs HIGH when vibration is detected (spring contacts close/open), outputs LOW when idle. Sensitivity is adjusted via an onboard potentiometer.
- Pin Choice: D2 (supports external interrupt INT0 if high-speed detection is required).

**SW-520D Tilt Sensor:**
- Interface: Digital input pin.
- Logic: Outputs LOW when upright (rolling balls close the circuit), outputs HIGH/floating when tilted (rolling balls move away, opening circuit).
- Pull-up Requirement: Internal pull-up resistor (`INPUT_PULLUP`) must be enabled on the Arduino pin since the sensor acts as a simple switch.
- Pin Choice: D3 (supports external interrupt INT1).

**Active Buzzer & LED:**
- Interface: Digital output pins.
- Logic: Active HIGH. Writing HIGH triggers the buzzer sound and illuminates the LED.
- Current Consideration: LED requires a 220Ω current-limiting resistor to protect the micro-controller pin. Active Buzzer module has built-in driver transistor, drawing minimal current from pin D6.

## Display Driver Stack

**Waveshare 1.3" SPI LCD:**
- Interface: Hardware SPI (MOSI -> D11, SCK -> D13) + GPIO Control (CS -> D10, RST -> D8, DC -> D7, BL -> D9).
- Refresh Cost: Clear and draw operations are synchronous and block the CPU. Full-screen redraws can take up to 100ms.

---
*Research: 2026-06-08*
