#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN 3
#define NUM_LEDS 16
#define BTN_PIN 6

Adafruit_NeoPixel ring(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  ring.begin();
  ring.show();
  pinMode(BTN_PIN, INPUT_PULLUP); 
}

void loop() {
  bool btn1 = digitalRead(BTN_PIN);  // HIGH = not pressed, LOW = pressed

  if (!btn1) {  // Button pressed
    for (int i = 0; i < NUM_LEDS; i++) {
      ring.setPixelColor(i, ring.Color(255, 0, 0));
    }
    ring.show();
  } else {
    // Button not pressed → turn LEDs off
    for (int i = 0; i < NUM_LEDS; i++) {
      ring.setPixelColor(i, 0);
    }
    ring.show();
  }

  delay(20); // Small debounce
}
