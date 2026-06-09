---
phase: quick-task
plan: 260609-p2n
subsystem: hardware-interfacing
tags: [sensors, arduino, input]
provides:
  - Inverted tilt sensor trigger logic to active-high
affects: [loop, sensors]
tech-stack:
  added: none
  patterns: [active-high pull-up input trigger]
key-files:
  created: []
  modified: [LCD_1inch3.ino]
key-decisions: []
duration: 5min
completed: 2026-06-09
status: complete
---

# Quick Task 260609-p2n: fix tilt sensor trigger logic Summary

**Inverted the tilt sensor pin logic to active-HIGH so that the alarm does not trigger immediately upon startup when the vehicle is stationary (upright).**

## Performance
- **Duration:** 5 min
- **Tasks:** 2
- **Files modified:** 1

## Accomplishments
- Fixed `LCD_1inch3.ino`'s tilt sensor check to trigger on `HIGH` rather than `LOW` (since the SW-520D / KY-020 is closed/LOW when upright and open/HIGH when tilted).
- Corrected the diagnostic print statements in `MODE_TEST_SENSORS` to report `TILTED (HIGH)` when `HIGH` and `Upright (LOW)` when `LOW`.

## Task Commits
1. **Task 1: Invert tilt sensor logic** - `17c69ff`

## Files Created/Modified
- `LCD_1inch3/LCD_1inch3.ino` - Updated tilt detection condition and debug logging.
