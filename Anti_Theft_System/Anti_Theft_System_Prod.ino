#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"

// Pin Definitions for Sensors and Alarms
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
const unsigned long SENSOR_HOLD_TIME = 2000;

unsigned long lastVibTriggerTime = 0;
unsigned long lastTiltTriggerTime = 0;
unsigned long tiltActiveStart = 0;
bool isTilted = false;

// Display Screen Drawing Helpers
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

void drawAlertScreen(const char* reason) {
  Paint_Clear(RED);
  // Center: "ALERT!" (6 chars * 17px/char = 102px. X = (240-102)/2 = 69)
  Paint_DrawString_EN(69, 80, "ALERT!", &Font24, RED, WHITE);
  // Center: "THEFT ATTEMPT!" (14 chars * 11px/char = 154px. X = (240-154)/2 = 43)
  Paint_DrawString_EN(43, 120, "THEFT ATTEMPT!", &Font16, RED, YELLOW);
  
  // Center the reason string
  int x = (240 - (strlen(reason) * 11)) / 2;
  Paint_DrawString_EN(x, 160, reason, &Font16, RED, WHITE);
}

void setup()
{
  Config_Init();
  
  // Initialize Serial for status output
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

  // Boot directly into Splash Screen
  currentState = STATE_SPLASH;
  splashStartTime = millis();
  drawSplashScreen();
  
  Serial.println("System Initialized. Starting splash screen.");
}

void loop()
{
  // 0. Handle Splash Screen Timer non-blockingly
  if (currentState == STATE_SPLASH) {
    if (millis() - splashStartTime >= 2000) {
      currentState = STATE_SAFE;
      drawSafeScreen();
      Serial.println("Vehicle Safe - Monitoring Active");
    }
    return; // Do not poll sensors during splash state
  }

  // 1. Poll the Vibration Sensor with hold time
  int rawVib = digitalRead(VIB_PIN);
  if (rawVib == HIGH) {
    lastVibTriggerTime = millis();
  }
  bool isVibrating = (lastVibTriggerTime > 0 && (millis() - lastVibTriggerTime < SENSOR_HOLD_TIME));

  // 2. Poll and Debounce the Tilt Sensor with hold time (Active HIGH - open circuit when tilted)
  int rawTilt = digitalRead(TILT_PIN);
  if (rawTilt == HIGH) {
    if (tiltActiveStart == 0) {
      tiltActiveStart = millis();
    }
    if (millis() - tiltActiveStart >= 50) {
      lastTiltTriggerTime = millis();
    }
  } else {
    tiltActiveStart = 0;
  }
  isTilted = (lastTiltTriggerTime > 0 && (millis() - lastTiltTriggerTime < SENSOR_HOLD_TIME));

  // 3. State Machine Transition Logic
  if (currentState == STATE_SAFE) {
    if (isVibrating || isTilted) {
      // Transition to Alert State
      currentState = STATE_ALERT;
      alertStartTime = millis();
      
      // Determine trigger reason
      const char* reason;
      if (isVibrating && isTilted) {
        reason = "VIB & TILT";
      } else if (isVibrating) {
        reason = "VIBRATION";
      } else {
        reason = "TILT DETECTED";
      }
      
      drawAlertScreen(reason);
      Serial.print("ALERT: Theft Attempt! Trigger: ");
      Serial.println(reason);
    }
  } else if (currentState == STATE_ALERT) {
    unsigned long elapsed = millis() - alertStartTime;
    
    // Check if the alarm duration has expired and conditions are safe
    if (elapsed >= MIN_ALERT_HOLD_TIME && !isVibrating && !isTilted) {
      // Transition back to Safe State
      currentState = STATE_SAFE;
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZ_PIN, LOW);
      drawSafeScreen();
      Serial.println("Vehicle Safe - Alarm Cleared");
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
