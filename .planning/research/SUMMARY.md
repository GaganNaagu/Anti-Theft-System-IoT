# Research Summary (Anti-Theft System)

**Analysis Date:** 2026-06-08

## Synthesis of Findings

### 1. Technology Choices
- **Hardware Integration**: Target platform is Arduino Uno with AVR runtime. SPI LCD uses pins 7-13. Pin assignments are safely assigned to D2 (Vibration), D3 (Tilt), D5 (LED), and D6 (Buzzer) to avoid SPI signal conflicts.
- **Library Reuse**: Standard `SPI.h` and the existing Waveshare `LCD_Driver` & `GUI_Paint` libraries are reused. The SSD1306 I2C OLED from the PDF guide is replaced by the 1.3" SPI LCD.

### 2. Feature Architecture
- Real-time vibration and tilt triggers are table-stake features.
- System transitions through three major states: Boot/Ready, Safe, and Alert.
- Visual alerts use a high-contrast red background on the 1.3" screen, while safe states display a green screen or text.

### 3. Engineering Challenges
- **Latency**: LCD writes block CPU execution. A transition-based drawing model is required to prevent rendering loops from blocking sensor reads.
- **Reliability**: A minimum alarm hold time (5s) prevents buzzer clicking on quick vibration pulses. The SW-520D input requires pull-up configuration and software debounce.

---
*Research synthesized: 2026-06-08*
