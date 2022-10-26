#include <Adafruit_NeoPixel.h>

#define LED_PIN   6  // any PWM capable pin
#define AUDIO_PIN A0  // input pin for audio signal 

#define NUM_LEDS 79
#define WAVE_LENGTH 30
#define COLOR_MAX 150
#define MAX_DELAY 150

#define MAX_VOLTAGE_LEVEL 200
#define C_LIMIT 75

#define DEBUG_MSG true

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_RGBW + NEO_KHZ800);
uint32_t lights[NUM_LEDS];

void setup()
{
  #if DEBUG_MSG
    Serial.begin(9600);
  #endif
  
  strip.begin();
  strip.setBrightness(20); // set brightness to n%

  turnOffPixels();
}

void loop()
{
  showWave(strip.Color(0, 255, 0, 10));
}

void showWave(uint32_t color) {
  uint32_t volume = 150;
  uint32_t waveIndex = 0;
  uint32_t calcDelay = 0;
  bool showColor = true;
  
  #if DEBUG_MSG
    Serial.println("START WAVE");
  #endif

  while (true) {
    int volume = analogRead(AUDIO_PIN);
    
    #if DEBUG_MSG
      Serial.println(volume);
    #endif
  
    waveIndex++;
    
    // switch show-color flag every n (= WAVE_LENGTH) iterations
    if(waveIndex % WAVE_LENGTH == 0) {
      showColor = !showColor;
      waveIndex = 0; // reset wave index to 0 otherwise the integer would get too large
    }

    // shift values in lights array to the right
    shiftLights(showColor, color, volume);

    // turn on pixels
    turnOnPixels();

    if(volume > 0) {
      calcDelay = calculateSpeed(volume);
      delay(calcDelay);
    } else {
      delay(10);
    }
    
    //Serial.println(calcDelay);
    
  }
}

uint32_t calculateSpeed(uint32_t volume) {
  // normalize voltage level on a scale from 1 to 9
  return min(100, 100 - ((volume - 0) * (100 - 1)/(MAX_VOLTAGE_LEVEL - 0)));
}

/*
 * initialize lights array
 * 1 = light is on
 * 0 = light is off
 * 
 * shift lights array to the right
 * this produces the wave effect
 */
void shiftLights(bool showColor, uint32_t color, uint32_t volume) {
  for (int i = NUM_LEDS - 1; i > 0; i--) {
    lights[i] = lights[i-1];
  }
    
  if (showColor) {
    lights[0] = calculateColor(color, volume);
  } else {
    lights[0] = 0;
  }
}

/*
 * calculate color of current LED
 * 
 * the color is based on the volume
 * low: green
 * intermediate: blue
 * high: red
 */
uint32_t calculateColor(uint32_t color, uint32_t volume) {
  uint8_t r, g, b;
  
  b = color >> 16;
  r = color >> 8;
  g = color;

  if (volume < C_LIMIT) {
    // green accent is strongest near the point from where on the colors get mixed (C_LIMIT)
    // sound at higher volume produces less green and more blue
    
    // green is the max saturation value (255) times the proportion of the current volume to the color switch point
    g = 255.0 * volume / C_LIMIT;
    return strip.Color(g, 0, 0, 40);
  } else if (volume < C_LIMIT * 2) {
    g = max(0, 255.0 - (255.0 * (volume - C_LIMIT)/ C_LIMIT));
    b = 255.0 * (volume - C_LIMIT) / C_LIMIT;
    return strip.Color(g, 0, b, 40);
  } else {
    b = max(0, 255.0 - (255.0 * (volume - C_LIMIT * 2) / C_LIMIT));
    r = min(255, 255.0 * (volume - C_LIMIT * 2) / C_LIMIT);
    return strip.Color(0, r, b, 10);
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
