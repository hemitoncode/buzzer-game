#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  const String name; // Human-readable name
};

// ------------------- Pin Definitions -------------------
const int BUZZERPIN = 12;
const int RESET_BTN_PIN = A5;
const int LEDPIN = 13;
const int BUZZER_BTN_PINS[] = {A3, A0};
const int SHOW_POINTS_BTN_PIN = 3;
const int INCREMENT_BTN_PIN = A1;

const int rs = 2, en = 4, d4 = 5, d5 = 8, d6 = 10, d7 = 7;
const int NUM_BUZZER_BTNS = sizeof(BUZZER_BTN_PINS) / sizeof(BUZZER_BTN_PINS[0]);
const int NUMPIXELS = 8;

// ------------------- Lib Setups -------------------
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// ------------------- Global Variables -------------------
int GAME_POINTS[NUM_BUZZER_BTNS] = {0};

const Color buttonColors[NUM_BUZZER_BTNS] = {
  {0, 0, 255, "Blue"},
  {0, 255, 0, "Green"}
};

// ------------------- Notes -------------------
#define NOTE_C4 262
#define NOTE_G3 196

// ------------------- Setup -------------------
void setup() {
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(RESET_BTN_PIN, INPUT_PULLUP);
  pinMode(INCREMENT_BTN_PIN, INPUT_PULLUP);
  pinMode(SHOW_POINTS_BTN_PIN, INPUT_PULLUP);

  for (int i = 0; i < NUM_BUZZER_BTNS; i++) {
    pinMode(BUZZER_BTN_PINS[i], INPUT_PULLUP);
  }

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Welcome!");
  lcd.setCursor(0, 1);
  lcd.print("Buzzer Game.");

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
void playTune(const int melody[], const int durations[], int size) {
  for (int i = 0; i < size; i++) {
    int noteDuration = 1000 / durations[i];
    tone(BUZZERPIN, melody[i], noteDuration);
    delay(noteDuration * 1.3);
    noTone(BUZZERPIN);
  }
}

void playBuzzTune() {
  const int beepFrequency = 1000;
  const int beepDuration = 250;
  const int pauseBetweenBeeps = 100;

  for (int i = 0; i < 3; i++) {
    tone(BUZZERPIN, beepFrequency, beepDuration);
    delay(beepDuration + pauseBetweenBeeps);
    noTone(BUZZERPIN);
  }
}

// ------------------- LCD Helpers -------------------
void showGamePoint(int points, String color) {
  clearDisplay();
  lcd.print(color + " score:");
  lcd.setCursor(0, 1);

  if (points == 1) {
    lcd.print(String(points) + " point!");
  } else {
    lcd.print(String(points) + " points!");
  }
}

void displayTeam(String color) {
  clearDisplay();
  lcd.print(color + " selected!");
  lcd.setCursor(0, 1);
}

void clearDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
}

// ------------------- Startup Animation -------------------
void startupLights() {
  uint32_t colors[] = {
    pixels.Color(0, 150, 0),
    pixels.Color(0, 0, 150),
    pixels.Color(150, 150, 0),
    pixels.Color(0, 150, 0),
    pixels.Color(0, 0, 150),
    pixels.Color(150, 150, 0),
    pixels.Color(0, 150, 0),
    pixels.Color(0, 0, 150)
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
  // --- Show Points Button ---
  if (digitalRead(SHOW_POINTS_BTN_PIN) == LOW) {
    for (int i = 0; i < NUM_BUZZER_BTNS; i++) {
      if (digitalRead(BUZZER_BTN_PINS[i]) == LOW) {
        showGamePoint(GAME_POINTS[i], buttonColors[i].name);
      }
    }
    delay(200);
    return;
  }

  // --- Increment Points ---
  if (digitalRead(INCREMENT_BTN_PIN) == LOW) {
    for (int i = 0; i < NUM_BUZZER_BTNS; i++) {
      if (digitalRead(BUZZER_BTN_PINS[i]) == LOW) {
        GAME_POINTS[i]++;
        const int melody[] = {NOTE_C4, NOTE_G3};
        const int durations[] = {4, 4};
        playTune(melody, durations, 2);
        showGamePoint(GAME_POINTS[i], buttonColors[i].name);
        delay(300);
        return;
      }
    }
  }

  // --- Reset Button ---
  if (digitalRead(RESET_BTN_PIN) == LOW) {
    clearDisplay();
    lcd.print("Resetting...");
    startupLights();

    for (int i = 0; i < NUM_BUZZER_BTNS; i++) {
      GAME_POINTS[i] = 0;
    }

    clearNeoPixel();
    lcd.clear();
    lcd.print("Scores reset!");
    delay(500);
    return;
  }

  // --- Buzzer Buttons ---
  for (int i = 0; i < NUM_BUZZER_BTNS; i++) {
    if (digitalRead(BUZZER_BTN_PINS[i]) == LOW) {
      displayTeam(buttonColors[i].name);
      allLights(buttonColors[i].r, buttonColors[i].g, buttonColors[i].b);
      playBuzzTune();
      delay(300);
    }
  }
}
