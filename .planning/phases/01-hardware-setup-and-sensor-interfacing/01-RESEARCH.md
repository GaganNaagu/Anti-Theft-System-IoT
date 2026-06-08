# Phase 01: Hardware Setup and Sensor Interfacing - Research

**Date:** 2026-06-08

## Technical Approach

### 1. Pin Configuration and I/O Modes
The pins for Phase 1 are configured in the `setup()` function:
- **D2 (SW-420 Vibration Sensor)**: Configured as `INPUT`.
- **D3 (SW-520D Tilt Sensor)**: Configured as `INPUT_PULLUP`. Since the tilt sensor is a rolling-ball type switch, it acts as a simple mechanical contact. Pulling the pin HIGH internally ensures a reliable logic level. When upright, the contact closes and pulls the pin to GND (reads `LOW`). When tilted, the contact opens, floating the pin to the internal pull-up voltage (reads `HIGH`).
- **D5 (LED)**: Configured as `OUTPUT`.
- **D6 (Active Buzzer)**: Configured as `OUTPUT`.

### 2. Debouncing the SW-520D Tilt Switch
Mechanical ball tilt switches bounce when shaken or moved slightly.
- **Approach**: Maintain a debounce counter. The sensor must be read consistently for `N` consecutive loop cycles (or a continuous duration of 50ms) before transitioning state. This is done without blocking the CPU via `millis()` timers.

### 3. Alarm State Machine and Timing
To prevent display rendering from blocking sensor polling, the system runs as a state-machine in the `loop()` function.

```cpp
enum AlarmState {
  STATE_SAFE,
  STATE_ALERT
};
AlarmState currentState = STATE_SAFE;
unsigned long alertTriggeredTime = 0;
const unsigned long MIN_ALERT_HOLD_TIME = 5000; // 5 seconds
```

#### Transition Logic:
- **SAFE -> ALERT**: Triggered immediately if Vibration (D2) reads `HIGH` or Tilt (D3) reads `HIGH` (after debounce).
  - Set `currentState = STATE_ALERT`.
  - Set `alertTriggeredTime = millis()`.
  - Log alert to Serial.
- **ALERT -> SAFE**: Triggered only if:
  - Milliseconds elapsed since trigger `millis() - alertTriggeredTime >= MIN_ALERT_HOLD_TIME`.
  - AND both sensors read safe (Vibration is `LOW`, Tilt is `LOW` after debounce).
  - Set `currentState = STATE_SAFE`.
  - Turn off alarm outputs.

### 4. Non-Blocking Pulsed Alarm Pattern
To implement a pulsed beep and flash pattern:
- Define `const unsigned long PULSE_INTERVAL = 500;` (500ms).
- In the `loop()`, if `currentState == STATE_ALERT`, check if `millis() % (2 * PULSE_INTERVAL) < PULSE_INTERVAL`.
  - If yes, write D5 and D6 `HIGH`.
  - If no, write D5 and D6 `LOW`.
- This creates a 1Hz beep/flash pattern without calling `delay(500)`.

### 5. Diagnostics Logging
- Logging uses the standard `Serial` interface at 9600 baud.
- A boolean flag `bool logPrinted = false` ensures the alert message is printed only once per alert event rather than flooding the console.

---
*Research complete: 2026-06-08*
