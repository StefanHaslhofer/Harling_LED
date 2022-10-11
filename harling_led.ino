#include <Adafruit_NeoPixel.h>

#define LED_PIN   6  // any PWM capable pin
#define AUDIO_PIN 0  // input pin for audio signal 

#define NUM_LEDS 79
#define WAVE_LENGTH 30
#define COLOR_MAX 150
#define MAX_DELAY 150

#define DEBUG_MSG false

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_RGBW + NEO_KHZ800);
uint32_t lights[NUM_LEDS];

void setup()
{
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(100); // set brightness to n%
}

void loop()
{
  showWave(strip.Color(20, COLOR_MAX, 20));
}

void showWave(uint32_t color) {
  int audioPressure = 1;
  int waveIndex = 0;
  bool showColor = true;
  
  #if DEBUG_MSG
    Serial.println("START WAVE");
  #endif
  
  turnOffPixels();

  while (true) {
    //int audioPressure = analogRead(AUDIO_PIN);
    waveIndex++;
    
    // switch show-color flag every n (= WAVE_LENGTH) iterations
    if(waveIndex % WAVE_LENGTH == 0) {
      showColor = !showColor;
      waveIndex = 0; // reset wave index to 0 otherwise the integer would get too large
    }

    // shift values in lights array to the right
    shiftLights(waveIndex, showColor, color);

    // turn on pixels
    turnOnPixels();
    delay(MAX_DELAY/audioPressure);
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
void shiftLights(int waveIndex, bool showColor, uint32_t color) {
  for (int i = NUM_LEDS - 1; i > 0; i--) {
    lights[i] = lights[i-1];
  }
    
  if (showColor) {
    lights[0] = calculateColor(waveIndex, color);
  } else {
    lights[0] = 0;
  }
}

/*
 * calculate color of current LED
 */
uint32_t calculateColor(int waveIndex, uint32_t color) {
  
  // 150 / WAVE_LENGTH = reduction per iteration
  // lights[i] equals the position in the wave

  // pos 0 in wave: no reduction
  // pos n in wave: reduction for each step times n

  return color;
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
 * sets strip color based on lights array
 * the brightness is determined by the position of the LED in a wave
 * 
 * edges are darker than the middle
 * this "smoothes" the wave effect
 */
void turnOnPixels()
{   
  for (int i = 0; i < NUM_LEDS; i++) {
    // turn on or off lights based on value in array
    strip.setPixelColor(i, lights[i]);
  }
    
  strip.show();
}
