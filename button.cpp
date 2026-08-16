#include "button.h"

bool buttonPress(int pin) {  //funkcija za pritisk knofa

  static bool pressAllow[64];
  bool buttonState;
  static bool lastButtonState[64];
  static bool initialized = false;

  if (!initialized) {
    for (int i = 0; i < 64; i++) {
      pressAllow[i] = 1;
      lastButtonState[i] = 1;
    }
    initialized = true;
  }

  buttonState = digitalRead(pin);

  if (buttonState == 0 && lastButtonState[pin] == 1 && pressAllow[pin] == true) {
    pressAllow[pin] = false;
    delay(20);
    buttonState = digitalRead(pin);

    if (buttonState == 0) {
      lastButtonState[pin] = buttonState;
      return true;
    }
  }
    if (buttonState == 1 && lastButtonState[pin] == 0) {
    delay(20);
    buttonState = digitalRead(pin);
    if (buttonState == 1) {
      pressAllow[pin] = true;
    }
  }

  lastButtonState[pin] = buttonState;

  return false;
}