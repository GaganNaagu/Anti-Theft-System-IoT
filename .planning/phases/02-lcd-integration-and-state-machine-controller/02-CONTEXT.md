# Phase 02: LCD Integration and State Machine Controller - Context

**Gathered:** 2026-06-08
**Status:** Ready for planning

<domain>
## Phase Boundary

This phase integrates the Waveshare 1.3-inch SPI LCD (240x240) into the anti-theft system's state machine. The display will show status transitions visually: a startup splash screen, a "Vehicle Safe" status, and a red "ALERT! Movement Detected" screen. All display updates must be non-blocking and strictly state-change triggered to keep sensor polling fast.

</domain>

<decisions>
## Implementation Decisions

### Startup Splash Screen
- **D-01:** Show a startup screen for exactly 2 seconds (2000ms) with a dark blue background, displaying "VEHICLE SECURE" in Font24 (White) and "System Ready" in Font16 (Yellow).

### Screen Visual Layouts
- **D-02:** **Safe State Screen:** A clean dark background (e.g. BLACK or DARKBLUE) displaying "STATUS: SAFE" in green (GREEN, Font24) and a subtitle "Monitoring..." in grey/white (LIGHTBLUE, Font16).
- **D-03:** **Alert State Screen:** A bright red background (RED) displaying "ALERT!" in bold white text (WHITE, Font24) and "THEFT ATTEMPT!" in yellow (YELLOW, Font16).

### Refresh Strategy & Non-Blocking Design
- **D-04:** Enforce state-change-triggered rendering: the display is cleared and drawn *exactly once* during state transitions (Startup -> Safe, Safe -> Alert, Alert -> Safe) to avoid continuous SPI writes that would block sensor polling.
- **D-05:** Keep the main loop highly responsive (no delays) by managing the 2-second startup splash timing using `millis()` instead of `delay()`.

### Code Separation
- **D-06:** All custom screens, text drawing, and rendering state checks must live exclusively in [LCD_1inch3.ino](file:///c:/Users/spast/Documents/Arduino/IoT/LCD_1inch3/LCD_1inch3.ino) to keep vendor libraries untouched.

</decisions>

<canonical_refs>
## Canonical References

**Downstream agents MUST read these before planning or implementing.**

### LCD Driver & Graphics APIs
- `LCD_1inch3/LCD_Driver.h` — LCD_Init, LCD_Clear, and raw register functions.
- `LCD_1inch3/GUI_Paint.h` — Paint_DrawString_EN, Paint_DrawRectangle, etc.
- `LCD_1inch3/fonts.h` — Available fonts (Font24, Font16, Font12, Font8).

</canonical_refs>

<code_context>
## Existing Code Insights

### Reusable Assets
- `LCD_Init()`, `LCD_SetBacklight(100)` — To initialize and power display backlight.
- `Paint_NewImage(Width, Height, Rotate, Color)` and `Paint_Clear(Color)` — To initialize rendering context and clear screen directly.
- `Paint_DrawString_EN(...)` — Primary text rendering primitive.

### Integration Points
- Draw startup splash in `setup()` but track time non-blockingly so `loop()` transitions to `STATE_SAFE` after 2000ms.
- Render "Safe" screen on transition to `STATE_SAFE`.
- Render "Alert" screen on transition to `STATE_ALERT`.

</code_context>

<specifics>
## Specific Ideas

- The display should be cleared with `Paint_Clear(Color)` before drawing the contents of the new state to prevent visual noise.

</specifics>

<deferred>
## Deferred Ideas

- Mobile/IoT notifications and remote buzzer controls are deferred to v2.

</deferred>

---
*Phase: 02-lcd-integration-and-state-machine-controller*
*Context gathered: 2026-06-08*
