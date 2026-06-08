#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

enum SystemState {
  STATE_SAFE,
  STATE_ALERT
};

SystemState currentState = STATE_SAFE;
unsigned long alertStartTime = 0;
const unsigned long MIN_ALERT_HOLD_TIME = 5000;

unsigned long tiltActiveStart = 0;
bool isTilted = false;

void setup()
{
  Config_Init();
  
  // Re-initialize Serial to 9600 baud to match the requirements
  Serial.begin(9600);
  Serial.println("System Initializing...");

  LCD_Init();
  LCD_Clear(WHITE);
  LCD_SetBacklight(100);
  
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, WHITE);
  Paint_Clear(WHITE);
  Paint_SetRotate(180);
  
  // Visual test pattern for startup
  Paint_DrawString_EN(30, 10, "123", &Font24, YELLOW, RED);  
  Paint_DrawString_EN(30, 34, "ABC", &Font24, BLUE, CYAN);
  Paint_DrawString_CN(50, 180, "微雪电子", &Font24CN, WHITE, RED);
  Paint_DrawRectangle(125, 10, 225, 58, RED, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  Paint_DrawCircle(180, 100, 25, BLACK, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  Paint_DrawImage(gImage_70X70, 20, 80, 70, 70); 

  Serial.println("System Ready");
}

void loop()
{
  // 1. Poll the Vibration Sensor
  int vibrationState = DEV_Digital_Read(DEV_VIB_PIN);

  // 2. Poll and Debounce the Tilt Sensor (Active LOW)
  int rawTilt = DEV_Digital_Read(DEV_TILT_PIN);
  if (rawTilt == LOW) {
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
      Serial.println("ALERT: Theft Attempt!");
    }
  } else if (currentState == STATE_ALERT) {
    unsigned long elapsed = millis() - alertStartTime;
    
    // Check if the alarm duration has expired and conditions are safe
    if (elapsed >= MIN_ALERT_HOLD_TIME && vibrationState == LOW && !isTilted) {
      // Transition back to Safe State
      currentState = STATE_SAFE;
      DEV_Digital_Write(DEV_LED_PIN, 0);
      DEV_Digital_Write(DEV_BUZZ_PIN, 0);
      Serial.println("Vehicle Safe");
    } else {
      // Non-blocking pulsed alert (1Hz: 500ms on, 500ms off)
      if ((millis() / 500) % 2 == 0) {
        DEV_Digital_Write(DEV_LED_PIN, 1);
        DEV_Digital_Write(DEV_BUZZ_PIN, 1);
      } else {
        DEV_Digital_Write(DEV_LED_PIN, 0);
        DEV_Digital_Write(DEV_BUZZ_PIN, 0);
      }
    }
  }
}
