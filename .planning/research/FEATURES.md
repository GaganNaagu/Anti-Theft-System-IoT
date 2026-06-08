# Research: Features (Anti-Theft System)

**Analysis Date:** 2026-06-08

## Feature Categorization

### Table Stakes (Must-Have for v1)
- **Real-Time Tamper Detection**: Instant trigger when vehicle is shaken (SW-420) or tilted/towed (SW-520D).
- **Local Alarm Output**: Loud audio warning (Buzzer) and high-visibility flashing alert (LED).
- **LCD Status Indicators**:
  - Initial splash: "System Ready" (lasts for 2 seconds on boot).
  - Normal state: "Vehicle Safe" (static text).
  - Alert state: Red-themed flashing screen displaying "ALERT! Movement Detected".
- **Serial Activity Log**: Sends timestamps and alert descriptions to the Serial monitor (9600 baud) for diagnostics.

### Differentiators (Future Scope)
- **SMS/Cellular Alerts**: Sends a text message to the owner's phone via a GSM module (e.g. SIM800L).
- **Cloud Dashboard**: ESP32 integration to update vehicle status to a central web portal.
- **Battery Monitoring**: Warns if the backup battery power is low.

### Anti-Features (Deliberately Excluded)
- **Interactive Touch Screen Control**: Excluded because the display is a simple 1.3" non-touch screen and the project is meant to act as a headless autonomous monitor unit.

---
*Research: 2026-06-08*
