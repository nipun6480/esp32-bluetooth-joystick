#include <Arduino.h>
#include <BleKeyboard.h>

// Bluetooth device name shown on phone
BleKeyboard bleKeyboard("ESP32 Snake Joystick", "Nipun", 100);

// Joystick pins
#define VRX 34
#define VRY 35
#define SW  32   // Middle button

// Joystick tuning
const int CENTER = 2048;
const int DEADZONE = 500;
const unsigned long SEND_INTERVAL_MS = 150;

unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  bleKeyboard.begin();

  pinMode(SW, INPUT_PULLUP);  // Button uses internal pull-up
}

void loop() {
  if (!bleKeyboard.isConnected()) {
    delay(50);
    return;
  }

  int x = analogRead(VRX);
  int y = analogRead(VRY);
  int btn = digitalRead(SW);   // LOW when pressed

  unsigned long now = millis();
  if (now - lastSend < SEND_INTERVAL_MS) return;

  // Left / Right
  if (x > CENTER + DEADZONE) {
    bleKeyboard.write(KEY_RIGHT_ARROW);
    lastSend = now;
  } else if (x < CENTER - DEADZONE) {
    bleKeyboard.write(KEY_LEFT_ARROW);
    lastSend = now;
  }

  // Up / Down
  if (y > CENTER + DEADZONE) {
    bleKeyboard.write(KEY_UP_ARROW);
    lastSend = now;
  } else if (y < CENTER - DEADZONE) {
    bleKeyboard.write(KEY_DOWN_ARROW);
    lastSend = now;
  }

  // Middle Button = ENTER
  if (btn == LOW) {
    bleKeyboard.write(KEY_RETURN);  // Enter key
    lastSend = now;
    delay(200); // simple debounce
  }
}
