# Requirements

**Analysis Date:** 2026-06-08

## v1 Requirements

### Hardware Configuration (HW)
- [ ] **HW-01**: Configure digital pin D2 as an INPUT pin for the SW-420 Vibration Sensor.
- [ ] **HW-02**: Configure digital pin D3 as an INPUT_PULLUP pin for the SW-520D Tilt Sensor to enable the internal pull-up resistor.
- [ ] **HW-03**: Configure digital pin D6 as an OUTPUT pin for the Active Buzzer.
- [ ] **HW-04**: Configure digital pin D5 as an OUTPUT pin for the Alert LED.

### System Initialization & Polling (SYS)
- [ ] **SYS-01**: Initialize hardware configurations, Serial communication (9600 baud), and SPI LCD on startup, displaying a "System Ready" splash screen for 2 seconds.
- [ ] **SYS-02**: Implement a non-blocking loop execution to continuously poll the Vibration (D2) and Tilt (D3) inputs.

### Alarm & Trigger Controls (ALRT)
- [ ] **ALRT-01**: Trigger the Buzzer (D6 -> HIGH) and LED (D5 -> HIGH) immediately when Vibration (D2 reads HIGH) or Tilt (D3 reads LOW) is detected.
- [ ] **ALRT-02**: Enforce a minimum holding time of 5 seconds (5000ms) for active alarms to prevent rapid pulsing/clicking of alerts.
- [ ] **ALRT-03**: Turn off Buzzer (D6 -> LOW) and LED (D5 -> LOW) after the alarm holding duration has expired and all sensors are in a quiet state.

### Display Rendering (DISP)
- [ ] **DISP-01**: Draw a red-themed alert warning screen showing "ALERT!" and "Movement Detected" exactly once when transitioning from Safe to Alert state.
- [ ] **DISP-02**: Draw a white/green-themed status screen showing "Vehicle Safe" exactly once when transitioning from Alert/Boot to Safe state.

### Diagnostics Logging (LOG)
- [ ] **LOG-01**: Log "ALERT: Theft Attempt!" to the serial interface once per alert event when the alert triggers.

## v2 Requirements (Deferred)
- **SMS-01**: Send real-time SMS alert notifications via a SIM800L GSM module.
- **CLOUD-01**: Stream system state and sensor metrics to a cloud dashboard using an ESP32 micro-controller.
- **BATT-01**: Monitor backup battery voltage and trigger low-power warnings.

## Out of Scope
- Support for SSD1306 I2C OLED — Excluded because we are reusing the Waveshare 1.3-inch SPI LCD.
- Complex user menus or configuration settings via display — Excluded due to AVR SRAM constraints (2KB).

## Traceability

| Requirement ID | Mapped Phase | Status |
|----------------|--------------|--------|
| HW-01          | —            | Pending|
| HW-02          | —            | Pending|
| HW-03          | —            | Pending|
| HW-04          | —            | Pending|
| SYS-01         | —            | Pending|
| SYS-02         | —            | Pending|
| ALRT-01        | —            | Pending|
| ALRT-02        | —            | Pending|
| ALRT-03        | —            | Pending|
| DISP-01        | —            | Pending|
| DISP-02        | —            | Pending|
| LOG-01         | —            | Pending|

---
*Last updated: 2026-06-08 after requirements definition*
