<!-- GSD:project-start source:PROJECT.md -->
## Project

**Smart Vehicle Monitoring and Anti-Theft System**

This project is an embedded low-cost anti-theft system that monitors a vehicle for suspicious activities. It detects physical tampering (shaking/vibration) and lifting (towing/tilt) and triggers real-time visual and audio alerts (on an LCD screen, LED, and buzzer) and prints events to the serial monitor.

**Core Value:** The system must reliably detect vehicle vibration or tilting and instantly trigger local audio and visual alarms.

### Constraints

- **Microcontroller SRAM Limit**: Arduino Uno has only 2KB of SRAM. We must avoid allocating large full-screen buffers in micro-controller RAM and instead write pixels directly to the LCD's display memory where possible, and avoid dynamic memory allocations (`malloc`) to prevent heap fragmentation.
- **Microcontroller Pin Budget**: SPI display utilizes pins 7, 8, 9, 10, 11, 13. Sensor and alarm outputs must be assigned to the remaining free pins (D2, D3, D5, D6).
<!-- GSD:project-end -->

<!-- GSD:stack-start source:codebase/STACK.md -->
## Technology Stack

## Languages
- C++ (Arduino Dialect) - All application logic, driver code, and GUI drawing routines.
- None.
## Runtime
- Arduino AVR Runtime - Designed for AVR-microcontrollers (e.g., ATmega328P on Arduino Uno/Nano).
- Uses AVR-specific features like `avr/pgmspace.h` for storing fonts and images in program memory (Flash) instead of SRAM.
- None (manual file inclusion).
## Frameworks
- Arduino Core SDK - Standard Arduino setup/loop execution model and standard GPIO/SPI APIs.
- None.
- Arduino IDE / CLI.
## Key Dependencies
- `SPI.h` (Built-in Arduino library) - Provides SPI communication interface to the LCD hardware driver.
- None.
## Configuration
- Hardcoded pin configurations in `DEV_Config.h`.
- Compile parameters are determined by the Arduino IDE board selection (AVR).
## Platform Requirements
- Arduino IDE or compatible build system.
- AVR Microcontroller (such as Arduino Uno, Nano, or similar) connected to a Waveshare 1.3inch LCD display module via SPI.
<!-- GSD:stack-end -->

<!-- GSD:conventions-start source:CONVENTIONS.md -->
## Conventions

## Naming Patterns
- PascalCase with underscores for layer files: `DEV_Config.cpp`/`DEV_Config.h`, `GUI_Paint.cpp`/`GUI_Paint.h`, `LCD_Driver.cpp`/`LCD_Driver.h`.
- Lowercase with numbers for font files: `font8.cpp`, `font12.cpp`, `font16.cpp`, `font20.cpp`, `font24.cpp`, `font24CN.cpp`.
- Lowercase for asset files: `image.cpp`/`image.h`.
- Prefixed PascalCase based on layers:
- Internal helper functions or register writers may use camelCase or mixed case: `LCD_WriteData_Byte(...)`, `LCD_WriteData_Word(...)`.
- PascalCase or Mixed Snake Case for variables: `Xpoint`, `Ypoint`, `Color_Background`, `Color_Foreground`.
- Prefix underscore is used for macros or local variables in header templates: `__xms`, `_pin`.
- Custom type aliases defined in `DEV_Config.h` to enforce portability:
- Structures are named in UPPERCASE (e.g. `PAINT`, `PAINT_TIME`) with `s` prefix sometimes used for variables: `sPaint_time`.
- Enums are written in UPPERCASE names and values (e.g., `DOT_PIXEL`, `LINE_STYLE_SOLID`).
## Code Style
- C++ standard formatting, typically 2 or 4 spaces or tab indentations (inconsistencies exist due to vendor-provided code).
- Semicolons are required.
- Curly braces are placed on the next line for functions/control statements:
- Pin mappings and hardware writes are fully capitalized: `DEV_CS_PIN`, `DEV_SPI_WRITE(...)`.
## Error Handling
- No exceptions (`try`/`catch`) are used, which is standard for Arduino AVR microcontrollers.
- Error codes are not returned; drawing functions fail silently if coordinates are out of bounds.
- Hardware failures during initialization do not block execution; they fail silently.
## Logging
- Custom logging macros defined in `Debug.h`:
- Uses standard Arduino `Serial` library.
<!-- GSD:conventions-end -->

<!-- GSD:architecture-start source:ARCHITECTURE.md -->
## Architecture

## Pattern Overview
- **Layered Design**: Clear separation between high-level drawing primitives, panel driver commands, and target hardware platform configurations.
- **Off-Screen Buffer Rendering**: Graphics are drawn to a RAM buffer first before being transferred to the LCD screen, allowing complex drawing operations without screen flickering.
- **Hardware Abstraction Layer (HAL)**: Hardware-specific code (SPI transfer, Pin I/O) is abstracted behind macros to allow easy porting to other platforms.
## Layers
- Purpose: Defines the main application logic, setup configurations, and coordinate drawing commands.
- Contains: `setup()`, `loop()`.
- Depends on: `GUI_Paint.h`, `LCD_Driver.h`, `image.h`.
- Used by: Arduino runtime system.
- Purpose: Implements software drawing primitives (lines, circles, text, images) on a RAM frame buffer.
- Contains: `Paint_NewImage()`, `Paint_DrawLine()`, `Paint_DrawCircle()`, `Paint_DrawString_EN()`, `Paint_DrawImage()`, etc.
- Depends on: `LCD_Driver.h`, `fonts.h`, `Debug.h`, `DEV_Config.h`.
- Used by: Application layer (`LCD_1inch3.ino`).
- Purpose: Communicates directly with the display controller chip (commands, configuration, write memory).
- Contains: `LCD_Init()`, `LCD_Clear()`, `LCD_WriteReg()`, `LCD_WriteData_Byte()`.
- Depends on: `DEV_Config.h`.
- Used by: Graphical Paint Layer (`GUI_Paint.cpp`) and Application Layer (`LCD_1inch3.ino`).
- Purpose: Wraps micro-controller physical GPIO pins and SPI communications.
- Contains: `Config_Init()`, pin definition macros (`DEV_CS_PIN`, `DEV_DC_PIN`, `DEV_RST_PIN`, `DEV_BL_PIN`), write macros (`DEV_Digital_Write`, `DEV_SPI_WRITE`).
- Depends on: `SPI.h`, `Debug.h`, standard Arduino libs.
- Used by: Hardware Driver Layer and Graphical Paint Layer.
## Data Flow
- The global `PAINT` structure (defined in `GUI_Paint.cpp`) keeps track of the active rendering context, including the pointer to the image buffer, width, height, rotation, and current color settings.
## Key Abstractions
- Purpose: Defines the properties of the frame buffer currently being drawn onto.
- Fields: `Image` (buffer pointer), `Width`, `Height`, `Rotate`, `Mirror`, `Color`.
- Pattern: State context struct.
- Purpose: Abstracts display controller command interface (e.g. ST7789 commands).
- Pattern: Hardware Driver.
## Entry Points
- Location: `LCD_1inch3.ino`
- Triggers: Micro-controller power-on or reset.
- Responsibilities: Initialize hardware, run graphics demos once in `setup()`.
## Error Handling
- Debug messages can be printed to the Serial monitor using `DEBUG()` macro defined in `Debug.h` (active if `DEBUG` macro is defined).
## Cross-Cutting Concerns
- System-wide logging setup utilizing a macro `DEBUG(...)` which resolves to `Serial.print(...)` when debugging is enabled.
<!-- GSD:architecture-end -->

<!-- GSD:skills-start source:skills/ -->
## Project Skills

No project skills found. Add skills to any of: `.agent/skills/`, `.agents/skills/`, `.cursor/skills/`, or `.github/skills/` with a `SKILL.md` index file.
<!-- GSD:skills-end -->

<!-- GSD:workflow-start source:GSD defaults -->
## GSD Workflow Enforcement

Before using Edit, Write, or other file-changing tools, start work through a GSD command so planning artifacts and execution context stay in sync.

Use these entry points:
- `/gsd-quick` for small fixes, doc updates, and ad-hoc tasks
- `/gsd-debug` for investigation and bug fixing
- `/gsd-execute-phase` for planned phase work

Do not make direct repo edits outside a GSD workflow unless the user explicitly asks to bypass it.
<!-- GSD:workflow-end -->



<!-- GSD:profile-start -->
## Developer Profile

> Profile not yet configured. Run `/gsd-profile-user` to generate your developer profile.
> This section is managed by `generate-claude-profile` -- do not edit manually.
<!-- GSD:profile-end -->
