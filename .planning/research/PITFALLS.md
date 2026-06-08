# Research: Pitfalls (Anti-Theft System)

**Analysis Date:** 2026-06-08

## Critical Pitfalls & Mitigations

### 1. Blocking Screen Redraws
- **Problem**: Redrawing the LCD screen takes ~100ms. If we redraw the screen continuously in the `loop()`, the CPU will spend 99% of its time pushing pixels, making it extremely likely to miss brief vibration pulses on the sensor pins.
- **Mitigation**: Implement a state-change listener. Only draw on the screen *once* when transitioning states (e.g. from Safe to Alert, or Alert to Safe).

### 2. Transient Sensor Pulses
- **Problem**: Vibration sensors (SW-420) output brief, sporadic HIGH pulses. If the alarm is turned off immediately when the pin returns to LOW, the buzzer will just click or chirp rather than sound a proper alarm.
- **Mitigation**: Implement a minimum alarm hold duration (e.g., 5000 milliseconds). Once an alert is triggered, the buzzer and red alert screen must stay active for at least 5 seconds before checking if it is safe to reset.

### 3. Tilt Switch Bouncing
- **Problem**: The SW-520D uses metal rolling balls. Vibrations or slight movements cause the balls to roll back and forth, producing multiple false state triggers (switch bouncing).
- **Mitigation**: Add a small software debounce window (e.g., 50-100ms) or require the tilt pin to stay active for multiple loop cycles before triggering the alarm.

### 4. Limited Arduino Uno SRAM (2KB)
- **Problem**: Attempting to allocate memory for full double-buffering will crash the Uno.
- **Mitigation**: Use direct-write LCD functions (`LCD_SetUWORD`) which draw directly to display memory, and avoid dynamically-allocated string buffers.

---
*Research: 2026-06-08*
