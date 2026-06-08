# Codebase Structure

**Analysis Date:** 2026-06-08

## Directory Layout

```
IoT/
├── .planning/                  # GSD Project planning metadata
│   └── codebase/              # Codebase analysis documents
├── LCD_1inch3/                 # Main Arduino project directory
│   ├── DEV_Config.cpp          # HAL implementations
│   ├── DEV_Config.h            # HAL pin configuration & macros
│   ├── Debug.h                 # Debug macros
│   ├── GUI_Paint.cpp           # Software rendering implementations
│   ├── GUI_Paint.h             # Paint engine structures & primitives
│   ├── LCD_1inch3.ino          # Application entry point
│   ├── LCD_Driver.cpp          # Screen registers and driver code
│   ├── LCD_Driver.h            # LCD driver declarations
│   ├── fonts.h                 # Font configurations
│   ├── font8.cpp               # 8-pixel font bitmaps
│   ├── font12.cpp              # 12-pixel font bitmaps
│   ├── font16.cpp              # 16-pixel font bitmaps
│   ├── font20.cpp              # 20-pixel font bitmaps
│   ├── font24.cpp              # 24-pixel font bitmaps
│   ├── font24CN.cpp            # 24-pixel Chinese font bitmaps
│   ├── image.cpp               # Hex pixel arrays for custom images
│   └── image.h                 # Declarations for image assets
└── .git/                       # Git repository metadata
```

## Directory Purposes

**.planning/codebase/**
- Purpose: Contains codebase mapping documents (like this one) created during project initialization.
- Contains: `STACK.md`, `INTEGRATIONS.md`, `ARCHITECTURE.md`, `STRUCTURE.md`, `CONVENTIONS.md`, `TESTING.md`, `CONCERNS.md`.

**LCD_1inch3/**
- Purpose: Contains all the source code for the Arduino application.
- Contains: `.ino`, `.cpp`, `.h` files.

## Key File Locations

**Entry Points:**
- `LCD_1inch3/LCD_1inch3.ino` - Main Arduino sketch entry containing `setup()` and `loop()`.

**Configuration:**
- `LCD_1inch3/DEV_Config.h` - Pin mapping config (CS, DC, RST, BL) and hardware access macros.
- `LCD_1inch3/Debug.h` - Debug logging macro settings.

**Core Logic:**
- `LCD_1inch3/GUI_Paint.cpp` - Drawing engine logic.
- `LCD_1inch3/LCD_Driver.cpp` - Hardware SPI setup and panel registers initialization.

**Assets & Fonts:**
- `LCD_1inch3/image.cpp` - Hardcoded 16-bit RGB565 image bitmaps.
- `LCD_1inch3/font*.cpp` - Monospaced pixel-font glyph bit arrays.

## Naming Conventions

**Files:**
- `LCD_*.cpp`/`LCD_*.h` - Driver-specific code.
- `DEV_*.cpp`/`DEV_*.h` - Device hardware configuration code.
- `GUI_*.cpp`/`GUI_*.h` - Graphical library modules.
- `font*.cpp`/`fonts.h` - Font asset definitions.
- `image.cpp`/`image.h` - Image asset definitions.

**Functions:**
- `Prefix_FunctionName` - PascalCase functions prefixed by their layer (e.g. `Paint_DrawCircle`, `Config_Init`, `LCD_SetCursor`).
- `LCD_FunctionName` - Driver API functions (e.g. `LCD_Init`, `LCD_Clear`).
- `DEV_MacroName` - SnakeCase macros for hardware commands (e.g. `DEV_Digital_Write`, `DEV_SPI_WRITE`).

## Where to Add New Code

**New Drawing Feature:**
- Add functions to `LCD_1inch3/GUI_Paint.cpp` and declare them in `LCD_1inch3/GUI_Paint.h`.

**New Font:**
- Create `LCD_1inch3/fontXX.cpp`, declare the font structure in `LCD_1inch3/fonts.h`, and reference it in the drawing engine.

**New Image Asset:**
- Generate C array using an image-to-hex converter tool, paste it in `LCD_1inch3/image.cpp`, and declare it in `LCD_1inch3/image.h`.

**New Target Board Support:**
- Add platform detection macros to `LCD_1inch3/DEV_Config.h` and write custom implementation blocks inside `LCD_1inch3/DEV_Config.cpp`.

---

*Structure analysis: 2026-06-08*
