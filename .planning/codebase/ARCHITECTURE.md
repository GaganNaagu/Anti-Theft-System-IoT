# Architecture

**Analysis Date:** 2026-06-08

## Pattern Overview

**Overall:** Layered Driver & GUI System for Embedded Display

**Key Characteristics:**
- **Layered Design**: Clear separation between high-level drawing primitives, panel driver commands, and target hardware platform configurations.
- **Off-Screen Buffer Rendering**: Graphics are drawn to a RAM buffer first before being transferred to the LCD screen, allowing complex drawing operations without screen flickering.
- **Hardware Abstraction Layer (HAL)**: Hardware-specific code (SPI transfer, Pin I/O) is abstracted behind macros to allow easy porting to other platforms.

## Layers

**Application Layer (`LCD_1inch3.ino`):**
- Purpose: Defines the main application logic, setup configurations, and coordinate drawing commands.
- Contains: `setup()`, `loop()`.
- Depends on: `GUI_Paint.h`, `LCD_Driver.h`, `image.h`.
- Used by: Arduino runtime system.

**Graphical Paint Layer (`GUI_Paint.cpp` / `GUI_Paint.h`):**
- Purpose: Implements software drawing primitives (lines, circles, text, images) on a RAM frame buffer.
- Contains: `Paint_NewImage()`, `Paint_DrawLine()`, `Paint_DrawCircle()`, `Paint_DrawString_EN()`, `Paint_DrawImage()`, etc.
- Depends on: `LCD_Driver.h`, `fonts.h`, `Debug.h`, `DEV_Config.h`.
- Used by: Application layer (`LCD_1inch3.ino`).

**Hardware Driver Layer (`LCD_Driver.cpp` / `LCD_Driver.h`):**
- Purpose: Communicates directly with the display controller chip (commands, configuration, write memory).
- Contains: `LCD_Init()`, `LCD_Clear()`, `LCD_WriteReg()`, `LCD_WriteData_Byte()`.
- Depends on: `DEV_Config.h`.
- Used by: Graphical Paint Layer (`GUI_Paint.cpp`) and Application Layer (`LCD_1inch3.ino`).

**Configuration Abstraction Layer (`DEV_Config.cpp` / `DEV_Config.h`):**
- Purpose: Wraps micro-controller physical GPIO pins and SPI communications.
- Contains: `Config_Init()`, pin definition macros (`DEV_CS_PIN`, `DEV_DC_PIN`, `DEV_RST_PIN`, `DEV_BL_PIN`), write macros (`DEV_Digital_Write`, `DEV_SPI_WRITE`).
- Depends on: `SPI.h`, `Debug.h`, standard Arduino libs.
- Used by: Hardware Driver Layer and Graphical Paint Layer.

## Data Flow

**Drawing and Rendering Flow:**

1. **Initialization**: `setup()` calls `Config_Init()` to set up SPI and pin directions, then calls `LCD_Init()` to initialize the LCD controller registers.
2. **Buffer Allocation**: `Paint_NewImage()` creates an off-screen buffer in RAM.
3. **Drawing Primitives**: The sketch calls `Paint_DrawRectangle()`, `Paint_DrawCircle()`, etc. These calculate pixel coordinates, apply rotation/mirroring, and set the corresponding bit/bytes in the RAM buffer.
4. **Drawing Images**: `Paint_DrawImage()` copies pre-stored flash bitmap data (`PROGMEM`) from `image.cpp` to the buffer.
5. **Physical Writing**: Pixels are sent from the buffer to the display over the hardware SPI bus.

**State Management:**
- The global `PAINT` structure (defined in `GUI_Paint.cpp`) keeps track of the active rendering context, including the pointer to the image buffer, width, height, rotation, and current color settings.

## Key Abstractions

**`PAINT` Structure (`GUI_Paint.h`):**
- Purpose: Defines the properties of the frame buffer currently being drawn onto.
- Fields: `Image` (buffer pointer), `Width`, `Height`, `Rotate`, `Mirror`, `Color`.
- Pattern: State context struct.

**Display Controller Driver (`LCD_Driver.h`):**
- Purpose: Abstracts display controller command interface (e.g. ST7789 commands).
- Pattern: Hardware Driver.

## Entry Points

**Arduino Main Entry (`LCD_1inch3.ino`):**
- Location: `LCD_1inch3.ino`
- Triggers: Micro-controller power-on or reset.
- Responsibilities: Initialize hardware, run graphics demos once in `setup()`.

## Error Handling

**Strategy:** Fail silently at runtime. Diagnostic logging is handled compilation-time.

**Patterns:**
- Debug messages can be printed to the Serial monitor using `DEBUG()` macro defined in `Debug.h` (active if `DEBUG` macro is defined).

## Cross-Cutting Concerns

**Debugging (`Debug.h`):**
- System-wide logging setup utilizing a macro `DEBUG(...)` which resolves to `Serial.print(...)` when debugging is enabled.

---

*Architecture analysis: 2026-06-08*
