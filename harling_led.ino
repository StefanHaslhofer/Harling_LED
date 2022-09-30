#include <Adafruit_NeoPixel.h>

#define LED_PIN   6  // any PWM capable pin
#define NUM_LEDS 79
#define WAVE_LENGTH 10

#define DEBUG_MSG true

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_RGBW + NEO_KHZ800);
uint8_t lights[NUM_LEDS];

void setup()
{
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(100); // set brightness to n%
}

void loop()
{
  //showMeteorPixels(1, 10, strip.Color(0, 150, 0));
  showWave(strip.Color(0, 150, 0));
}

void showWave(uint32_t color) {
  #if DEBUG_MSG
    Serial.println("START WAVE");
  #endif
  
  turnOffPixels();
  
  int waveIndex = 0;
  bool showColor = true;
  
  for (int i = 0; true; i++) {
    waveIndex++;
    // switch show-color flag every n (= WAVE_LENGTH) iterations
    if(waveIndex % WAVE_LENGTH == 0) {
      showColor = !showColor;
    }
    
    shiftLights(showColor);
    turnOnPixels(color);
    delay(1);
  }
}

/*
 * initialize lights array
 * 1 = light is on
 * 0 = light is off
 * 
 * shift lights array to the right
 * this produces the wave effect
 */
void shiftLights(bool showColor) {
  for (int i = NUM_LEDS - 1; i > 0; i--) {
    lights[i] = lights[i-1];
  }

  if (showColor) {
    lights[0] = 1;
  } else {
    lights[0] = 0;
  }
}

/*
 * sets strip color to 000 (off)
 */
void turnOffPixels()
{
  for (int i = 0; i < NUM_LEDS; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
  strip.show();
}

/*
 * sets strip color based on parameter
 */
void turnOnPixels(uint32_t color)
{  
  for (int i = 0; i < NUM_LEDS; i++) {
    Serial.write("a");
    // turn on or off lights based on value in array
    if(lights[i] == 1) {
      strip.setPixelColor(i, color);
    } else {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
  }
  strip.show();
}
