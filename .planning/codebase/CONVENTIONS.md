# Coding Conventions

**Analysis Date:** 2026-06-08

## Naming Patterns

**Files:**
- PascalCase with underscores for layer files: `DEV_Config.cpp`/`DEV_Config.h`, `GUI_Paint.cpp`/`GUI_Paint.h`, `LCD_Driver.cpp`/`LCD_Driver.h`.
- Lowercase with numbers for font files: `font8.cpp`, `font12.cpp`, `font16.cpp`, `font20.cpp`, `font24.cpp`, `font24CN.cpp`.
- Lowercase for asset files: `image.cpp`/`image.h`.

**Functions:**
- Prefixed PascalCase based on layers:
  - `Paint_DrawPoint(...)` (GUI Layer)
  - `LCD_Init(...)` (Driver Layer)
  - `Config_Init(...)` (HAL Layer)
- Internal helper functions or register writers may use camelCase or mixed case: `LCD_WriteData_Byte(...)`, `LCD_WriteData_Word(...)`.

**Variables:**
- PascalCase or Mixed Snake Case for variables: `Xpoint`, `Ypoint`, `Color_Background`, `Color_Foreground`.
- Prefix underscore is used for macros or local variables in header templates: `__xms`, `_pin`.

**Types:**
- Custom type aliases defined in `DEV_Config.h` to enforce portability:
  - `UBYTE` -> `uint8_t` (8-bit unsigned)
  - `UWORD` -> `uint16_t` (16-bit unsigned)
  - `UDOUBLE` -> `uint32_t` (32-bit unsigned)
- Structures are named in UPPERCASE (e.g. `PAINT`, `PAINT_TIME`) with `s` prefix sometimes used for variables: `sPaint_time`.
- Enums are written in UPPERCASE names and values (e.g., `DOT_PIXEL`, `LINE_STYLE_SOLID`).

## Code Style

**Formatting:**
- C++ standard formatting, typically 2 or 4 spaces or tab indentations (inconsistencies exist due to vendor-provided code).
- Semicolons are required.
- Curly braces are placed on the next line for functions/control statements:
  ```cpp
  void setup()
  {
    Config_Init();
  }
  ```

**Macros:**
- Pin mappings and hardware writes are fully capitalized: `DEV_CS_PIN`, `DEV_SPI_WRITE(...)`.

## Error Handling

**Patterns:**
- No exceptions (`try`/`catch`) are used, which is standard for Arduino AVR microcontrollers.
- Error codes are not returned; drawing functions fail silently if coordinates are out of bounds.
- Hardware failures during initialization do not block execution; they fail silently.

## Logging

**Framework:**
- Custom logging macros defined in `Debug.h`:
  ```cpp
  #define DEBUG 1 // Define to enable serial printing
  #if DEBUG
      #define DEV_DEBUG(_str) Serial.print(_str)
  #else
      #define DEV_DEBUG(_str)
  #endif
  ```
- Uses standard Arduino `Serial` library.

---

*Convention analysis: 2026-06-08*
