#include <Adafruit_NeoPixel.h>

// ------------------- Pin Definitions -------------------
const int BUZZERPIN = 2;
const int RESET_BTN_PIN = 4;
const int LEDPIN = 6;
const int BUZZER_BTN_PINS[] = {8, 9, 10};
const int NUM_BUZZER_BTNS = sizeof(BUZZER_BTN_PINS) / sizeof(BUZZER_BTN_PINS[0]);
const int INCREMENT_BTN_PIN = 11;
const int NUMPIXELS = 8;

// ------------------- NeoPixel Setup -------------------
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

// ------------------- Notes -------------------
#define NOTE_C4 262
#define NOTE_G3 196
#define NOTE_A3 220
#define NOTE_B3 247

// ------------------- Button Colors & Points -------------------
int buttonColors[NUM_BUZZER_BTNS][3] = {
  {0, 0, 255},     // Blue
  {255, 255, 0},   // Yellow
  {0, 255, 0}      // Green
};

int buttonPoints[NUM_BUZZER_BTNS] = {0};
bool prevButtonState[NUM_BUZZER_BTNS] = {HIGH, HIGH, HIGH};
int lastPressedButton = 0; // Tracks last pressed button for INCREMENT

// ------------------- Setup -------------------
void setup() {
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(RESET_BTN_PIN, INPUT_PULLUP);
  pinMode(INCREMENT_BTN_PIN, INPUT_PULLUP);

  for (int i = 0; i < NUM_BUZZER_BTNS; i++) {
    pinMode(BUZZER_BTN_PINS[i], INPUT_PULLUP);
  }

  pixels.begin();
  clearNeoPixel();
}

// ------------------- NeoPixel Helpers -------------------
void clearNeoPixel() {
  pixels.clear();
  pixels.show();
}

void allLights(int r, int g, int b) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
}

// ------------------- Sound Helpers -------------------
void playStartupTune() {
  int melody[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3};
  int durations[] = {4, 8, 8, 4};

  for (int i = 0; i < 4; i++) {
    int noteDuration = 1000 / durations[i];
    tone(BUZZERPIN, melody[i], noteDuration);
    delay(noteDuration * 1.3);
    noTone(BUZZERPIN);
  }
}

void playBuzzTune(int r = 0, int g = 0, int b = 255) {
  const int beepFrequency = 1000;
  const int beepDuration = 750;
  const int pauseBetweenBeeps = 100;

  for (int i = 0; i < 3; i++) {
    tone(BUZZERPIN, beepFrequency, beepDuration);
    delay(beepDuration + pauseBetweenBeeps);
    noTone(BUZZERPIN);
  }
}

// ------------------- Startup Animation -------------------
void startupLights() {
  uint32_t colors[] = {
    pixels.Color(0, 150, 0), pixels.Color(0, 0, 150), pixels.Color(150, 150, 0), pixels.Color(0, 150, 0),
    pixels.Color(0, 0, 150), pixels.Color(150, 150, 0), pixels.Color(0, 150, 0), pixels.Color(0, 0, 150)
  };

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.clear();
    pixels.setPixelColor(i, colors[i]);
    pixels.show();
    delay(100);
  }
}

// ------------------- Main Loop -------------------
void loop() {
  // --- Handle Reset Button ---
  if (digitalRead(RESET_BTN_PIN) == LOW) {
    startupLights();
    playStartupTune();
    clearNeoPixel();

    for (int i = 0; i < NUM_BUZZER_BTNS; i++) {
      buttonPoints[i] = 0;  // Reset points
    }
    delay(200); // debounce
    return;
  }

  // --- Handle Buzzer Buttons ---
  for (int i = 0; i < NUM_BUZZER_BTNS; i++) {
    bool currState = digitalRead(BUZZER_BTN_PINS[i]);

    // Detect button press (HIGH -> LOW)
    if (prevButtonState[i] == HIGH && currState == LOW) {
      allLights(buttonColors[i][0], buttonColors[i][1], buttonColors[i][2]);
      playBuzzTune(buttonColors[i][0], buttonColors[i][1], buttonColors[i][2]);
      buttonPoints[i]++;
      lastPressedButton = i; // Save last pressed button
    }

    prevButtonState[i] = currState; // Save state
  }

  // --- Handle Increment Button (only affects last pressed button) ---
  if (digitalRead(INCREMENT_BTN_PIN) == LOW) {
    buttonPoints[lastPressedButton]++;
    delay(200); // debounce
  }
}
