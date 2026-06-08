# Research: Architecture (Anti-Theft System)

**Analysis Date:** 2026-06-08

## System Architecture

```
                       +-------------------+
                       |    Arduino Uno    |
                       |   (AVR Core SDK)  |
                       +---------+---------+
                                 |
         +-----------------------+-----------------------+
         |                       |                       |
  [SPI Interface]         [Digital Pins]          [Digital Pins]
         |                       |                       |
         v                       v                       v
+--------+--------+     +--------+--------+     +--------+--------+
| Waveshare 1.3"  |     | SW-420 (Vib)    |     | Active Buzzer   |
| LCD (ST7789)    |     | SW-520D (Tilt)  |     | Alert LED       |
+-----------------+     +-----------------+     +-----------------+
```

## Execution Flow & Timing

Because the LCD drawing routines are synchronous and block the microcontroller for up to 100ms, continuous drawing in the loop will severely degrade the responsiveness of the sensors.

To address this, we define a **State-Machine Architecture**:

```
+-------------------+      Vibration / Tilt       +-------------------+
|                   | --------------------------> |                   |
|   VEHICLE_SAFE    |                             |   VEHICLE_ALERT   |
|                   | <-------------------------- |                   |
+-------------------+      Timeout (e.g. 5s)      +-------------------+
```

### State Variables:
- `systemState`: Enumerate `STATE_READY`, `STATE_SAFE`, `STATE_ALERT`.
- `lastAlertTime`: Stores the millis() timestamp when the last vibration/tilt event was detected.

### Loop Architecture:
1. **Poll Sensors**: Continuously read pins D2 and D3.
2. **State Transition Check**:
   - If in `STATE_SAFE` and sensor reads trigger:
     - Transition to `STATE_ALERT`.
     - Turn on Buzzer & LED.
     - **Draw screen once** with the "ALERT! Movement Detected" design.
     - Log to Serial.
   - If in `STATE_ALERT` and sensors are quiet, check if holding time (5 seconds) has elapsed:
     - Transition to `STATE_SAFE`.
     - Turn off Buzzer & LED.
     - **Draw screen once** with the "Vehicle Safe" design.
     - Log to Serial.
3. **Flashing Alerts (Optional)**: If in `STATE_ALERT`, toggle the LED and display colors every 500ms using non-blocking timer checks (`millis()`).

---
*Research: 2026-06-08*
