# Arduino IoT Smart Vehicle Anti-Theft System

An Arduino-based smart vehicle monitoring and anti-theft system. The project features an integrated non-blocking state machine that polls a vibration sensor and a tilt sensor, updating status screens on a Waveshare 1.3" SPI LCD and triggering audio/visual alarms (Pulsed Buzzer & LED) during security alerts. It also includes an interactive Serial diagnostic mode for independent component testing.

---

## 🛠 Features

* **Non-blocking State Machine**: System moves dynamically between **Splash**, **Safe (Monitoring)**, and **Alert** states without blocking sensor polling.
* **Smart Alert Reason Detection**: Displays the exact trigger cause (`VIBRATION`, `TILT DETECTED`, or `VIB & TILT`) on the LCD screen and prints it over the Serial Monitor when an alarm occurs.
* **Interactive Diagnostics Menu**: Send commands (`0-5`) via Serial Monitor to isolate and test components:
  * `0`: Normal Mode (Runs the full integrated anti-theft system)
  * `1`: Test LED Only (1Hz pulsing)
  * `2`: Test Buzzer Only (1Hz pulsing)
  * `3`: Test LCD Screen Only (cycles screens every 2s)
  * `4`: Test Sensors Only (prints real-time numeric states and statuses to Serial)
  * `5`: Return to Standby Menu

---

## 🔌 Hardware Connections

### 📺 Waveshare 1.3" SPI LCD Screen
| Pin Name | Arduino Pin | Description |
| :--- | :--- | :--- |
| **VCC** | 5V / 3.3V | Power Supply |
| **GND** | GND | Ground |
| **DIN** | D11 | SPI MOSI (Data) |
| **CLK** | D13 | SPI SCK (Clock) |
| **CS** | D10 | SPI Chip Select |
| **DC** | D7 | Data / Command Select |
| **RST** | D8 | Reset Pin |
| **BL** | D9 | Backlight Control |

### 🚨 Sensors & Alarms
| Component | Arduino Pin | Configuration & Logic |
| :--- | :--- | :--- |
| **Vibration Sensor (SW-420)** | **D2** | Digital Input. Reads `1` (HIGH) when vibration is detected, `0` (LOW) when idle. |
| **Tilt Sensor (SW-520D)** | **D3** | Digital Input (`INPUT_PULLUP`). Upright state closes contacts to GND (`0`/LOW). Tilted state opens the circuit, pulling the pin to `1` (HIGH). |
| **Status LED** | **D5** | Digital Output. Pulses at 1Hz on alarm. |
| **Buzzer** | **D6** | Digital Output. Pulses at 1Hz on alarm. |

*Note: Custom pins were carefully selected to avoid conflicts with the SPI bus pins used by the LCD display.*

---

## 🚀 How to Run the Project

1. Connect the hardware according to the pins above.
2. Open the main sketch `LCD_1inch3/LCD_1inch3.ino` in the **Arduino IDE**.
3. Compile and upload the sketch to your board (e.g., Arduino Uno or Nano).
4. Open the Arduino **Serial Monitor** and set the baud rate to **9600**.
5. Select a test mode from the console or press `0` to launch the security monitoring system.
