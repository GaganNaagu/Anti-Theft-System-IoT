# Phase 01: Hardware Setup and Sensor Interfacing - Discussion Log

> **Audit trail only.** Do not use as input to planning, research, or execution agents.
> Decisions are captured in CONTEXT.md — this log preserves the alternatives considered.

**Date:** 2026-06-08
**Phase:** 01-hardware-setup-and-sensor-interfacing
**Areas discussed:** Input Reading Method, Tilt Debounce Strategy, Alarm Output Pattern

---

## Input Reading Method

| Option | Description | Selected |
|--------|-------------|----------|
| Polling | Read the sensor pins sequentially in the main `loop()` (Recommended - simpler, less noise sensitive). | ✓ |
| Interrupts | Use hardware interrupts INT0/INT1 on D2/D3 to capture high-speed pulses. | |

**User's choice:** Polling (Recommended).
**Notes:** Decided to poll sensors in the loop to avoid interrupt glitches from electrical noise.

---

## Tilt Debounce Strategy

| Option | Description | Selected |
|--------|-------------|----------|
| Software Delays | Use `delay()` to block loop and check sensor again. | |
| Consecutive Checks | Use non-blocking state checks across loop iterations (Recommended - keeps loop responsive). | ✓ |

**User's choice:** Consecutive Checks (Recommended).
**Notes:** Keeps the loop running fast without blocking delays.

---

## Alarm Output Pattern

| Option | Description | Selected |
|--------|-------------|----------|
| Continuous | Hold Buzzer and LED constantly HIGH during active alert. | |
| Pulsed | Flash LED and beep Buzzer on and off periodically (Recommended - higher visibility). | ✓ |

**User's choice:** Pulsed (Recommended).
**Notes:** Increases the urgency and noticeability of the alarm.

---

## the agent's Discretion

- Precise debounce timing (e.g. 50-100ms) and baud rate setup (9600) is left to downstream planning.

## Deferred Ideas

- Integrated display screen state changes (Safe/Alert screens) deferred to Phase 2.
