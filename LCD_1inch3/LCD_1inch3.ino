#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

// Custom Pin Definitions for Sensors and Alarms (lives entirely in .ino)
#define VIB_PIN  2
#define TILT_PIN 3
#define LED_PIN  5
#define BUZZ_PIN 6

enum SystemState {
  STATE_SPLASH,
  STATE_SAFE,
  STATE_ALERT
};

SystemState currentState = STATE_SPLASH;
unsigned long splashStartTime = 0;
unsigned long alertStartTime = 0;
const unsigned long MIN_ALERT_HOLD_TIME = 5000;

unsigned long tiltActiveStart = 0;
bool isTilted = false;

// Test Mode Definitions for Hardware Diagnostic
enum TestMode {
  MODE_IDLE,         // Standby on startup, wait for command
  MODE_NORMAL,       // Normal integrated state machine
  MODE_TEST_LED,     // Test only the LED (pulsing at 1Hz)
  MODE_TEST_BUZZ,    // Test only the Buzzer (pulsing at 1Hz)
  MODE_TEST_LCD,     // Test only the LCD screen (cycling screens)
  MODE_TEST_SENSORS  // Test only the sensor inputs (printing raw status)
};

TestMode currentTestMode = MODE_IDLE;

// Test Timing Variables
unsigned long lastLcdTestChange = 0;
int lcdTestStep = 0;
unsigned long lastSensorPrint = 0;

// Display Screen Drawing Helpers
void drawStandbyScreen() {
  Paint_Clear(BLACK);
  // Center: "DIAGNOSTIC MODE" (15 chars * 11px/char = 165px. X = (240-165)/2 = 37)
  Paint_DrawString_EN(37, 90, "DIAGNOSTIC MODE", &Font16, BLACK, WHITE);
  // Center: "Send 0-4 over Serial" (20 chars * 5px/char = 100px. X = (240-100)/2 = 70)
  Paint_DrawString_EN(70, 130, "Send 0-4 over Serial", &Font8, BLACK, YELLOW);
}

void drawSplashScreen() {
  Paint_Clear(DARKBLUE);
  // Center: "VEHICLE SECURE" (14 chars * 17px/char = 238px. X = (240-238)/2 = 1)
  Paint_DrawString_EN(1, 90, "VEHICLE SECURE", &Font24, DARKBLUE, WHITE);
  // Center: "System Ready" (12 chars * 11px/char = 132px. X = (240-132)/2 = 54)
  Paint_DrawString_EN(54, 130, "System Ready", &Font16, DARKBLUE, YELLOW);
}

void drawSafeScreen() {
  Paint_Clear(BLACK);
  // Center: "STATUS: SAFE" (12 chars * 17px/char = 204px. X = (240-204)/2 = 18)
  Paint_DrawString_EN(18, 100, "STATUS: SAFE", &Font24, BLACK, GREEN);
  // Center: "Monitoring..." (13 chars * 11px/char = 143px. X = (240-143)/2 = 48)
  Paint_DrawString_EN(48, 140, "Monitoring...", &Font16, BLACK, LIGHTBLUE);
}

void drawAlertScreen() {
  Paint_Clear(RED);
  // Center: "ALERT!" (6 chars * 17px/char = 102px. X = (240-102)/2 = 69)
  Paint_DrawString_EN(69, 90, "ALERT!", &Font24, RED, WHITE);
  // Center: "THEFT ATTEMPT!" (14 chars * 11px/char = 154px. X = (240-154)/2 = 43)
  Paint_DrawString_EN(43, 130, "THEFT ATTEMPT!", &Font16, RED, YELLOW);
}

// Print diagnostic test menu over Serial
void printMenu() {
  Serial.println("\n===== SYSTEM TEST MENU =====");
  Serial.println("0 : Normal Mode (Full Integrated Anti-Theft System)");
  Serial.println("1 : Test LED Only (1Hz pulsing)");
  Serial.println("2 : Test Buzzer Only (1Hz pulsing)");
  Serial.println("3 : Test LCD Screen Only (cycles screens every 2s)");
  Serial.println("4 : Test Sensors Only (prints raw states to Serial)");
  Serial.println("5 : Return to Standby Menu");
  Serial.println("============================");
}

void setup()
{
  Config_Init();
  
  // Re-initialize Serial to 9600 baud to match the requirements
  Serial.begin(9600);
  Serial.println("System Initializing...");

  // Initialize sensors and alarm outputs
  pinMode(VIB_PIN, INPUT);
  pinMode(TILT_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZ_PIN, LOW);

  LCD_Init();
  LCD_Clear(WHITE);
  LCD_SetBacklight(100);
  
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, WHITE);
  Paint_Clear(WHITE);
  Paint_SetRotate(180);

  // Draw Standby screen on boot and display menu
  drawStandbyScreen();

  Serial.println("System Ready in Standby. Select mode from Serial Monitor.");
  printMenu();
}

void loop()
{
  // Check for incoming serial commands to change modes
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    // Clear outputs before switching modes
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZ_PIN, LOW);
    
    switch(cmd) {
      case '0':
        currentTestMode = MODE_NORMAL;
        currentState = STATE_SPLASH;
        splashStartTime = millis();
        drawSplashScreen();
        Serial.println("\n[MODE] Normal Mode selected (starting splash).");
        printMenu();
        break;
      case '1':
        currentTestMode = MODE_TEST_LED;
        Serial.println("\n[MODE] LED Test Mode (Buzzer & LCD idle).");
        break;
      case '2':
        currentTestMode = MODE_TEST_BUZZ;
        Serial.println("\n[MODE] Buzzer Test Mode (LED & LCD idle).");
        break;
      case '3':
        currentTestMode = MODE_TEST_LCD;
        lcdTestStep = 0;
        lastLcdTestChange = millis();
        drawSplashScreen();
        Serial.println("\n[MODE] LCD Test Mode (LED & Buzzer idle).");
        break;
      case '4':
        currentTestMode = MODE_TEST_SENSORS;
        lastSensorPrint = millis();
        Serial.println("\n[MODE] Sensor Reading Test Mode (Alarms disabled).");
        break;
      case '5':
      case 'm':
        currentTestMode = MODE_IDLE;
        drawStandbyScreen();
        Serial.println("\n[MODE] Standby Menu. Select test mode.");
        printMenu();
        break;
      default:
        // Ignore newline or other characters
        break;
    }
  }

  // Execute behavior based on current mode
  if (currentTestMode == MODE_IDLE) {
    // Standby: keep outputs LOW
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZ_PIN, LOW);
  }
  else if (currentTestMode == MODE_TEST_LED) {
    // Pulse LED at 1Hz (500ms on, 500ms off)
    if ((millis() / 500) % 2 == 0) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
  } 
  else if (currentTestMode == MODE_TEST_BUZZ) {
    // Pulse Buzzer at 1Hz (500ms on, 500ms off)
    if ((millis() / 500) % 2 == 0) {
      digitalWrite(BUZZ_PIN, HIGH);
    } else {
      digitalWrite(BUZZ_PIN, LOW);
    }
  } 
  else if (currentTestMode == MODE_TEST_LCD) {
    // Cycle screens every 2000ms
    if (millis() - lastLcdTestChange >= 2000) {
      lastLcdTestChange = millis();
      lcdTestStep = (lcdTestStep + 1) % 3;
      if (lcdTestStep == 0) {
        drawSplashScreen();
        Serial.println("  [LCD Test] Showing Splash Screen");
      } else if (lcdTestStep == 1) {
        drawSafeScreen();
        Serial.println("  [LCD Test] Showing Safe Screen");
      } else {
        drawAlertScreen();
        Serial.println("  [LCD Test] Showing Alert Screen");
      }
    }
  } 
  else if (currentTestMode == MODE_TEST_SENSORS) {
    // Print Raw readings every 500ms
    if (millis() - lastSensorPrint >= 500) {
      lastSensorPrint = millis();
      int vibVal = digitalRead(VIB_PIN);
      int tiltVal = digitalRead(TILT_PIN);
      Serial.print("  [Sensors] Vib: ");
      Serial.print(vibVal == HIGH ? "VIBRATING (HIGH)" : "Idle (LOW)");
      Serial.print(" | Tilt: ");
      Serial.println(tiltVal == HIGH ? "TILTED (HIGH)" : "Upright (LOW)");
    }
  } 
  else {
    // MODE_NORMAL: Runs the full integrated anti-theft system
    
    // 0. Handle Splash Screen Timer non-blockingly
    if (currentState == STATE_SPLASH) {
      if (millis() - splashStartTime >= 2000) {
        currentState = STATE_SAFE;
        drawSafeScreen();
        Serial.println("Vehicle Safe");
      }
      return; // Do not poll sensors during splash state
    }

    // 1. Poll the Vibration Sensor
    int vibrationState = digitalRead(VIB_PIN);

    // 2. Poll and Debounce the Tilt Sensor (Active HIGH - open circuit when tilted)
    int rawTilt = digitalRead(TILT_PIN);
    if (rawTilt == HIGH) {
      if (tiltActiveStart == 0) {
        tiltActiveStart = millis();
      }
      if (millis() - tiltActiveStart >= 50) {
        isTilted = true;
      }
    } else {
      tiltActiveStart = 0;
      isTilted = false;
    }

    // 3. State Machine Transition Logic
    if (currentState == STATE_SAFE) {
      if (vibrationState == HIGH || isTilted) {
        // Transition to Alert State
        currentState = STATE_ALERT;
        alertStartTime = millis();
        drawAlertScreen();
        Serial.println("ALERT: Theft Attempt!");
      }
    } else if (currentState == STATE_ALERT) {
      unsigned long elapsed = millis() - alertStartTime;
      
      // Check if the alarm duration has expired and conditions are safe
      if (elapsed >= MIN_ALERT_HOLD_TIME && vibrationState == LOW && !isTilted) {
        // Transition back to Safe State
        currentState = STATE_SAFE;
        digitalWrite(LED_PIN, LOW);
        digitalWrite(BUZZ_PIN, LOW);
        drawSafeScreen();
        Serial.println("Vehicle Safe");
      } else {
        // Non-blocking pulsed alert (1Hz: 500ms on, 500ms off)
        if ((millis() / 500) % 2 == 0) {
          digitalWrite(LED_PIN, HIGH);
          digitalWrite(BUZZ_PIN, HIGH);
        } else {
          digitalWrite(LED_PIN, LOW);
          digitalWrite(BUZZ_PIN, LOW);
        }
      }
    }
  }
}
