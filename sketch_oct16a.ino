#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  const String name;   // Human-readable name
};

// ------------------- Pin Definitions -------------------
const int BUZZERPIN = 12;
const int RESET_BTN_PIN = A5;
const int LEDPIN = 13;
const int BUZZER_BTN_PINS[] = {A3, A0};
const int SHOW_POINTS_BTN_PIN = A2;
const int INCREMENT_BTN_PIN = A1;

const int rs = 2, en = 4, d4 = 5, d5 = 8, d6 = 10, d7 = 7;

const int NUM_BUZZER_BTNS = sizeof(BUZZER_BTN_PINS) / sizeof(BUZZER_BTN_PINS[0]);
const int NUMPIXELS = 8;

// ------------------- Lib Setups -------------------
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// ------------------- Global Variables -------------------

int GAME_POINTS[NUM_BUZZER_BTNS] = {0};
int lastPressedButtonIdx = -1;

const Color buttonColors[NUM_BUZZER_BTNS] = {
  {0, 0, 255, "Blue"},     // Blue
  {0, 255, 0, "Green"}      // Green
};

// ------------------- Notes -------------------
// 3rd octave
#define NOTE_C3  130
#define NOTE_CS3 138
#define NOTE_D3  146
#define NOTE_DS3 155
#define NOTE_E3  164
#define NOTE_F3  174
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 207
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247

// 4th octave
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494

// 5th octave
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 830
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988


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
void playTune(int melody[], int durations[], int size) {

  for (int i = 0; i < size; i++) {
    int noteDuration = 1000 / durations[i];
    tone(BUZZERPIN, melody[i], noteDuration);
    delay(noteDuration * 1.3);
    noTone(BUZZERPIN);
  }
}

void playBuzzTune() {
  const int beepFrequency = 1000;
  const int beepDuration = 750;
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
  lcd.setCursor(0,1);
  lcd.print(String(points) + " points!");
}

void clearDisplay() {
  lcd.setCursor(0, 0); 
  lcd.print("                "); // Print 16 spaces to clear the line
  
  lcd.setCursor(0, 1); 
  lcd.print("                "); // Print 16 spaces to clear the line

  lcd.setCursor(0, 0);
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
  if (digitalRead(SHOW_POINTS_BTN_PIN) == LOW) {
    for (int i = 0; i < NUM_BUZZER_BTNS; i++) {
      if (digitalRead(BUZZER_BTN_PINS[i]) == LOW) {
        showGamePoint(GAME_POINTS[i], buttonColors[i].name);
      }
    }

    // Break out of full loop to start a new process
    return;
  }


  // --- Handle Reset Button ---
  if (digitalRead(RESET_BTN_PIN) == LOW) {
    clearDisplay();
    startupLights();
const int melody[] = {
  NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5,  // arpeggio up
  NOTE_C5, NOTE_G4, NOTE_E4, NOTE_C4,  // arpeggio down
  NOTE_D4, NOTE_F4, NOTE_A4, NOTE_D5,  // second theme
  NOTE_D5, NOTE_A4, NOTE_F4, NOTE_D4   // repeat down
};    

const int durations[] = {
  4, 4, 4, 2,   // first arpeggio
  4, 4, 4, 2,   // descending
  4, 4, 4, 2,   // second theme up
  4, 4, 4, 2    // second theme down
};

const int size = sizeof(melody) / sizeof(melody[0]);

    playTune(melody, durations, 4);
    clearNeoPixel();

    for (int i = 0; i < NUM_BUZZER_BTNS; i++) {
      GAME_POINTS[i] = 0;  // Reset points
    }

    lastPressedButtonIdx = -1;
    delay(200);
    return;
  }

  // --- Handle Buzzer Buttons ---
  for (int i = 0; i < NUM_BUZZER_BTNS; i++) {
    bool currState = digitalRead(BUZZER_BTN_PINS[i]);

    // Detect button press (HIGH -> LOW)
    if (currState == LOW) {
      allLights(buttonColors[i].r, buttonColors[i].g, buttonColors[i].b);
      playBuzzTune();
      lastPressedButtonIdx = i; // Save last pressed button
    }
  }

  // --- Handle Increment Button (only affects last pressed button) ---
  if (digitalRead(INCREMENT_BTN_PIN) == LOW) {
    if (lastPressedButtonIdx == -1) {
      const int melody[] = {NOTE_B3, NOTE_A3};
      const int durations[] = {8, 4};
      const int size = 2;
      playTune(melody, durations, size);
    }

    GAME_POINTS[lastPressedButtonIdx]++;

    const int melody[] = {NOTE_C4, NOTE_G3};
    const int durations[] = {4, 4};
    const int size = 2;
    playTune(melody, durations, size);
  }
}
