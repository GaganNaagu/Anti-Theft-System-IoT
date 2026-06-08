# Roadmap: Smart Vehicle Monitoring and Anti-Theft System

## Overview

This project is structured into two main phases to take the system from a basic hardware sensor demo to a fully integrated, state-controlled anti-theft system with optimized LCD visual alerts.

## Phases

- [ ] **Phase 1: Hardware Setup and Sensor Interfacing** - Wire and configure the Vibration and Tilt sensors, active buzzer, and LED, validating inputs and alarm output logic over Serial.
- [ ] **Phase 2: LCD Integration and State Machine Controller** - Integrate the 1.3-inch SPI display, show status transitions, and enforce non-blocking execution to keep sensor polling fast.

## Phase Details

### Phase 1: Hardware Setup and Sensor Interfacing
**Goal**: Wire the SW-420 and SW-520D sensors, buzzer, and LED to the Arduino, and implement the basic alert-triggering logic with Serial logging.
**Depends on**: Nothing (first phase)
**Requirements**: HW-01, HW-02, HW-03, HW-04, SYS-02, ALRT-01, ALRT-02, ALRT-03, LOG-01
**Success Criteria**:
  1. Vibration sensor (D2) triggers HIGH state on physical shaking.
  2. Tilt sensor (D3) triggers LOW state on physical tilting.
  3. Active buzzer (D6) and LED (D5) activate instantly on sensor triggers.
  4. Alarm outputs stay active for at least 5 seconds (5000ms) after a trigger event.
  5. Alert events are printed to the Serial monitor at 9600 baud.
**Plans**: 1 plan

Plans:
- [x] 01-01: Implement sensor polling, alarm outputs, and timing logic with Serial logging.

### Phase 2: LCD Integration and State Machine Controller
**Goal**: Configure the Waveshare 1.3" SPI LCD to display the system states and implement state-change rendering logic to prevent display updates from blocking the main loop.
**Depends on**: Phase 1
**Requirements**: SYS-01, DISP-01, DISP-02
**Success Criteria**:
  1. LCD displays "System Ready" on startup for 2 seconds.
  2. LCD updates to "Vehicle Safe" exactly once when transitioning to a safe state.
  3. LCD updates to red "ALERT! Movement Detected" exactly once when transitioning to an alert state.
  4. Display writes are strictly state-change triggered to avoid blocking the main sensor polling loop.
**Plans**: 1 plan

Plans:
- [ ] 02-01: Integrate LCD initialization, splash screens, and state-change display updates.

## Progress

| Phase | Plans Complete | Status | Completed |
|-------|----------------|--------|-----------|
| 1. Hardware Setup | 0/1 | Not started | - |
| 2. LCD Integration | 0/1 | Not started | - |

---
*Roadmap created: 2026-06-08*
