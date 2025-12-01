#include <Adafruit_NeoPixel.h>

#define NEOPIXEL1_PIN 3
#define NEOPIXEL2_PIN 8
#define NUM_LEDS 16
#define BTN1_PIN 6
#define BTN2_PIN 10

Adafruit_NeoPixel ring1(NUM_LEDS, NEOPIXEL1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ring2(NUM_LEDS, NEOPIXEL2_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  ring1.begin();
  ring2.begin();
  ring1.show();
  ring2.show();
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
}

void loop() {
  bool btn1 = digitalRead(BTN1_PIN);
  bool btn2 = digitalRead(BTN2_PIN);
  
  if (!btn1) { 
    showColor(255, 0, 0);
  } else if (!btn2) {
    showColor(0, 255, 0);
  } 
}

void removeColor():
  for (int i = 0; i < NUM_LEDS; i++) {
    ring1.setPixelColor(i, ring1.Color(0, 0, 0));
    ring2.setPixelColor(i, ring2.Color(0, 0, 0));  
  }
  
  ring1.show();
  ring2.show();

void showColor(int red, int green, int blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    ring1.setPixelColor(i, ring1.Color(red, green, blue));
    ring2.setPixelColor(i, ring2.Color(red, green, blue));
  }
  
  ring1.show();
  ring2.show();
  delay(5000);

  removeColor();
}
