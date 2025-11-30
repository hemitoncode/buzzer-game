#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN 3 // Data pin connected to the NeoPixel ring
#define NUM_LEDS 16    // NeoPixel ring has 16 LEDs

#define BTN_PIN 6 // Data pin connected for button
Adafruit_NeoPixel ring(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  ring.begin();
  ring.show(); // Initialize all pixels to 'off'
  pinMode(BTN_PIN, INPUT);    // sets the digital pin 7 as input

}

void loop() {
  bool btn1 = digitalRead(BTN_PIN)

  if (!btn1) {
    for (int i = 0; i < NUM_LEDS; i++) {
      ring.setPixelColor(i, ring.Color(255, 0, 0)); 
    }
    ring.show();
    delay(1000);
  }
  
  for (int i = 0; i < NUM_LEDS; i++) {
    ring.setPixelColor(i, ring.Color(0, 0, 0));
  }
  ring.show();
  delay(1000);
}
