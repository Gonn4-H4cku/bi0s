#include <BleKeyboard.h>

BleKeyboard bleKeyboard;
const int buttonPin = 0; // GPIO 0

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();

  // Initialize GPIO 0 as input with an internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  if (bleKeyboard.isConnected()) {
    // Check if the button is pressed (LOW means pressed due to pull-up)
    if (digitalRead(buttonPin) == LOW) {
      Serial.println("Button pressed, executing sequence...");

      // 1. Open Terminal (Ctrl + Alt + T)
      bleKeyboard.press(KEY_LEFT_CTRL);
      bleKeyboard.press(KEY_LEFT_ALT);
      bleKeyboard.press('t');
      delay(100);
      bleKeyboard.releaseAll();

      delay(1000); // Small delay to allow the terminal to open

      // 2. Type "Hello, World!"
      bleKeyboard.print("Hello, World");
      delay(10);
      bleKeyboard.press(KEY_LEFT_SHIFT);
      delay(10);
      bleKeyboard.print("1");
      delay(10);
      bleKeyboard.releaseAll();
      delay(10);
      bleKeyboard.print(" from BLE HID");
      delay(10);
      // 3. Delay for 5 seconds (5000 milliseconds)
      delay(5000);

      // 4. Close Terminal (Ctrl + Shift + Q)
      bleKeyboard.press(KEY_LEFT_CTRL);
      bleKeyboard.press(KEY_LEFT_SHIFT);
      bleKeyboard.press('q');
      delay(100);
      bleKeyboard.releaseAll();

      // Debounce delay to prevent multiple triggers due to button bounce
      delay(1000);
    }
  }

  // Small delay to avoid excessive polling
  delay(50);
}
