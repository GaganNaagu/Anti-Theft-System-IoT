# Codebase Concerns

**Analysis Date:** 2026-06-08

## Tech Debt

**Dynamic Allocation in `Paint_DrawFloatNum`:**
- Issue: Uses `malloc()` and `free()` dynamically to format decimal numbers.
- File: `LCD_1inch3/GUI_Paint.cpp` (lines 615-625)
- Why: Done to slice trailing zeroes or dots off string-formatted double values.
- Impact: Heap fragmentation on low-RAM AVR systems (like ATmega328P with only 2KB SRAM) can lead to silent crashes.
- Fix approach: Replace with a static buffer on the stack (e.g., `char pStr[32]`) and standard string index modifications.

**Hardcoded Display Settings in Driver:**
- Issue: Initialization registers and timings are hardcoded directly in functions.
- File: `LCD_1inch3/LCD_Driver.cpp`
- Why: Typical for direct vendor sample codes.
- Impact: Difficult to adapt to other screen sizes, controller models, or orientations without editing internal driver files.
- Fix approach: Create a config structure passed to `LCD_Init()` or extract panel register maps to a separate configuration header.

## Known Bugs

None documented, but silent coordinate overflows are handled by returning from `Paint_SetPixel()` rather than asserting or reporting errors.

## Security Considerations

**Physical SPI Attacks:**
- Risk: Since the microcontroller relies on standard SPI bus (MOSI, CLK) lines to send pixel data, a hardware bus sniffer could extract whatever is written to the display.
- Current mitigation: None (not a typical concern for local microcontroller demo boards, but important if handling passwords/sensitive data).
- Recommendations: Implement software encryption if communicating with secure systems, though generally unnecessary for simple status displays.

## Performance Bottlenecks

**SPI Transfer Overhead:**
- Problem: Individual byte transfers for commands and data are done synchronously using `SPI.transfer` macros one by one.
- File: `LCD_1inch3/DEV_Config.h` (line 66)
- Cause: Synchronous polling of the SPI status register is slow.
- Improvement path: Implement SPI DMA (Direct Memory Access) transfers if using a more advanced board (like ESP32 or STM32), or transfer in block segments.

**Double Buffer Clearing Loop:**
- Problem: Clearing the display with `Paint_Clear()` iterates over all pixels sequentially sending color data one pixel at a time via SPI commands.
- File: `LCD_1inch3/GUI_Paint.cpp` (lines 174-182)
- Measurement: Visible screen refresh wipe when clearing.
- Cause: No block-fill SPI commands used.
- Improvement path: Configure the LCD window address once and stream the color data in a fast loop without redefining coordinates per pixel.

## Fragile Areas

**Buffer Overflow in `Paint_DrawNum` / `Paint_DrawFloatNum`:**
- File: `LCD_1inch3/GUI_Paint.cpp` (lines 570, 610)
- Why fragile: Uses static buffer `ARRAY_LEN` of 50. While formatting large floats or integers, if the length of the string representation exceeds this limit, it can lead to memory corruption.
- Common failures: Garbage characters printed or micro-controller crash.
- Safe modification: Use `snprintf` with buffer bounds checks instead of `dtostrf`.

## Scaling Limits

**AVR RAM Constraints (SRAM):**
- Current capacity: Arduino Uno has 2KB SRAM.
- Limit: A full 240x240 RGB565 frame buffer requires `240 * 240 * 2 = 115,200` bytes (112.5 KB).
- Symptoms at limit: It is physically impossible to allocate a full off-screen buffer on AVR chips. The current code works around this by calling `LCD_SetUWORD` inside `Paint_SetPixel()` to write directly to the LCD controller's onboard RAM instead of allocating a large local array in the microcontroller's RAM.
- Scaling path: If full off-screen double buffering is required, upgrade the microcontroller to ESP32, STM32, or similar with at least 128KB of RAM.

## Test Coverage Gaps

**Visual Assets Validation:**
- What's not tested: The validity of pixel matrices in `font*.cpp` and hex arrays in `image.cpp` is never verified.
- Risk: Corruption of font data or image hex strings results in garbled screen outputs.
- Priority: Medium
- Difficulty to test: Requires manual checking of image dimensions or a helper tool to render arrays back to image formats.

---

*Concerns audit: 2026-06-08*
*Update as issues are fixed or new ones discovered*
