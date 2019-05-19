#include <FastLED.h>
#define LED_PIN         7
#define NUM_LEDS        12
#define PROGCHANGE_BTN  2
#define POWER_SWITCH    3

CRGB leds[NUM_LEDS];
int progNum = 0;
int lastBtnState = 0;


// The colors we want to show, in order for the transition
int colors[7][3] = {
  {255,   0,    0},
  {255, 255,    0},
  {0,   255,    0},
  {0,   255,  255},
  {0,     0,  255},
  {255,   0,  255},
  {255, 255,  255}
};


// ---------------------- INITIALISATION ----------------------

void setup() {
  randomSeed(analogRead(0));
  pinMode(PROGCHANGE_BTN, INPUT);
  pinMode(POWER_SWITCH, INPUT);
  FastLED.addLeds<WS2811, LED_PIN, GRB>(leds, NUM_LEDS);
  clearLEDs();
}



// ---------------------- MAIN LOOP ----------------------

void loop() {
  int btnState = digitalRead(PROGCHANGE_BTN);
  int turnedOn = digitalRead(POWER_SWITCH);
  boolean justChangedMode = false;

  if (turnedOn == 0){
    clearLEDs();
    FastLED.show();
    return;
  }

  
  // Button is pressed
  if (lastBtnState == 0 && btnState == 1) {
    justChangedMode = true;
    
    clearLEDs();
    progNum = (progNum + 1) % 11;
  }
  lastBtnState = btnState;


  // Animation programs
  switch (progNum) {
    case (0):
      setColorToAll(colors[progNum]);
      break;
    case (1):
      setColorToAll(colors[progNum]);
      break;
    case (2):
      setColorToAll(colors[progNum]);
      break;
    case (3):
      setColorToAll(colors[progNum]);
      break;
    case (4):
      setColorToAll(colors[progNum]);
      break;
    case (5):
      setColorToAll(colors[progNum]);
      break;
    case (6):
      setColorToAll(colors[progNum]);
      break;
    case (7):
      colorWheel();
      break;
    case (8):
      colorWheelAnim();
      break;
    case (9):
      randomColors();
      break;
    case (10):
      runningColor();
      break;
  }
}



// ---------------------- CUSTOM ANIMATIONS ----------------------

// All LEDs go through the color wheel defined
void colorWheel() {
  CRGB nextColor = nextColorOnWheel(leds[0], 1);
  
  setColorToAll(nextColor.r, nextColor.g, nextColor.b);
  delay(25);
}

// LEDs go through wheel, but in a snake like fashion
void colorWheelAnim() {
  CRGB nextColor = nextColorOnWheel(leds[0], 50);

  for(int i=NUM_LEDS-1; i > 0; i--) {
    leds[i] = leds[i-1];
  }

  leds[0] = nextColor;
  
  FastLED.show();
  delay(75);
}


// Random color on every 3 LEDs
int randomColorHold = 0;
void randomColors() {
  int r, g, b;

  if (randomColorHold == 0) {
    randomColorHold = 10000;  // Hold this color for 10 secs
    r = random(255);
    g = random(255);
    b = random(255);
    for (int i = 0; i < 3; i++) {
      leds[i] = CRGB(r, g, b);
    }
    r = random(255);
    g = random(255);
    b = random(255);
    for (int i = 3; i < 6; i++) {
      leds[i] = CRGB(r, g, b);
    }
    r = random(255);
    g = random(255);
    b = random(255);
    for (int i = 6; i < 9; i++) {
      leds[i] = CRGB(r, g, b);
    }
    r = random(255);
    g = random(255);
    b = random(255);
    for (int i = 9; i < 12; i++) {
      leds[i] = CRGB(r, g, b);
    }
    FastLED.show();
  }
  
  delay(50);
  randomColorHold -= 50;
  if (randomColorHold < 0){
    randomColorHold = 0;
  }
}

// Single color runs through every LED
int runningActLED = 0;
CRGB runningActColor;
void runningColor(){
    if (runningActLED == 0) {
      runningActColor = CRGB(random(255), random(255), random(255));
    }

    clearLEDs();
    leds[runningActLED] = runningActColor;
    
    runningActLED ++;
    if (runningActLED >= NUM_LEDS) {
      runningActLED = 0;
    }
    
    FastLED.show();
    delay(100);
}



// ---------------------- UTILITY FUNCTIONS ----------------------

int colorWheelActColor = 0;
CRGB nextColorOnWheel(CRGB color, int increment) {
//  CRGB newColor = color;
  int r = color.r;
  int g = color.g;
  int b = color.b;
  
  if (r == colors[colorWheelActColor][0] &&
      g == colors[colorWheelActColor][1] &&
      b == colors[colorWheelActColor][2]) {
    colorWheelActColor ++;
    colorWheelActColor = colorWheelActColor % 7;
  }

  if (color.r < colors[colorWheelActColor][0]) {
    r += increment;
  }
  if (color.g < colors[colorWheelActColor][1]) {
    g += increment;
  }
  if (color.b < colors[colorWheelActColor][2]) {
    b += increment;
  }
  
  if (color.r > colors[colorWheelActColor][0]) {
    r -= increment;
  }
  if (color.g > colors[colorWheelActColor][1]) {
    g -= increment;
  }
  if (color.b > colors[colorWheelActColor][2]) {
    b -= increment;
  }

  if (r > 255) r = 255;
  if (g > 255) g = 255;
  if (b > 255) b = 255;
  if (r < 0)   r = 0;
  if (g < 0)   g = 0;
  if (b < 0)   b = 0;

  return CRGB(r, g, b);
}


void clearLEDs() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
}

void setColorToAll(int r, int g, int b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(r, g, b);
  }
  FastLED.show();
}
void setColorToAll(int color[]) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(color[0], color[1], color[2]);
  }
  FastLED.show();
}
