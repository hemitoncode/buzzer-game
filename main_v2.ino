#include <Adafruit_NeoPixel.h>

#define PIN 6          // Data pin connected to the NeoPixel ring
#define NUM_LEDS 16    // NeoPixel ring has 16 LEDs

Adafruit_NeoPixel ring(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  ring.begin();
  ring.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Turn all LEDs on (red example)
  for (int i = 0; i < NUM_LEDS; i++) {
    ring.setPixelColor(i, ring.Color(255, 0, 0)); // Red, change values if you want
  }
  ring.show();
  delay(1000);

  // Turn all LEDs off
  for (int i = 0; i < NUM_LEDS; i++) {
    ring.setPixelColor(i, ring.Color(0, 0, 0));
  }
  ring.show();
  delay(1000);
}
