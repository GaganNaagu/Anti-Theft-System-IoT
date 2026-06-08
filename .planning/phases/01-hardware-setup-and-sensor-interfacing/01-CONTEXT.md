# Phase 01: Hardware Setup and Sensor Interfacing - Context

**Gathered:** 2026-06-08
**Status:** Ready for planning

<domain>
## Phase Boundary

This phase delivers the core logic for interfacing physical sensors (SW-420 Vibration, SW-520D Tilt) and outputs (Active Buzzer, LED) with the Arduino, including polling loops, state-change alarms, and Serial event logging. It does not integrate the LCD screen (which is deferred to Phase 2).

</domain>

<decisions>
## Implementation Decisions

### Hardware Configuration & Pin Mappings
- **D-01:** Connect sensors and outputs to pins: Vibration Sensor (SW-420) to digital pin D2, Tilt Switch (SW-520D) to digital pin D3, Active Buzzer to digital pin D6, and LED to digital pin D5.
- **D-02:** Configure the Tilt pin (D3) as `INPUT_PULLUP` to engage the internal pull-up resistor.

### Input Interfacing & Debouncing
- **D-03:** Poll the sensor pins sequentially in the main `loop()` instead of attaching hardware interrupts, minimizing susceptibility to high-frequency electrical noise triggers.
- **D-04:** Implement a non-blocking consecutive-checks logic to debounce the SW-520D tilt sensor, requiring it to remain active for a brief period before triggering the alarm.

### Alarm & Alert Outputs
- **D-05:** Implement a pulsed alert pattern (e.g. 500ms on, 500ms off) for the Buzzer and LED using `millis()` timing to raise alert visibility and urgency.
- **D-06:** Enforce a minimum alert hold duration of 5 seconds (5000ms) once triggered to prevent buzzer stuttering/clicking on brief vibration triggers.

### the agent's Discretion
- Downstream planning agent may determine the exact debounce count/time (e.g., 50-100ms) and precise baud rate configuration (9600).

</decisions>

<canonical_refs>
## Canonical References

**Downstream agents MUST read these before planning or implementing.**

### Configuration & Logging
- `LCD_1inch3/DEV_Config.h` — Pin configurations and GPIO macros.
- `LCD_1inch3/Debug.h` — Custom serial logging macros.

</canonical_refs>

<code_context>
## Existing Code Insights

### Reusable Assets
- `DEV_Digital_Read(pin)` / `DEV_Digital_Write(pin, value)` macros in `DEV_Config.h` — Used for reading sensors and driving alert outputs.
- `DEV_DEBUG(str)` macro in `Debug.h` — Used for logging event status to the Serial console.

### Established Patterns
- Arduino setup/loop structure.
- Configuration variables and macros declared in `DEV_Config.h` and implemented in `DEV_Config.cpp`.

### Integration Points
- Sensor pins configuration should be added to `DEV_Config.h` and initialized inside `Config_Init()`.
- Polling and state machine logic will reside inside `LCD_1inch3.ino`.

</code_context>

<specifics>
## Specific Ideas

- Normal state prints "Vehicle Safe" over Serial, while alert state prints "ALERT: Theft Attempt!" once per event.

</specifics>

<deferred>
## Deferred Ideas

- Visual LCD graphics update (e.g. "ALERT! Movement Detected" red screen, "Vehicle Safe" green/white screen) is deferred to Phase 2.

</deferred>

---
*Phase: 01-hardware-setup-and-sensor-interfacing*
*Context gathered: 2026-06-08*
