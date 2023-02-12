#include <Adafruit_NeoPixel.h>

// declare spectrum shield pin connections
#define STROBE 4
#define RESET 6
#define DC_ONE A0
#define DC_TWO A1 

#define NUM_STRIPS 6
#define NUM_LEDS 30
#define WAVE_LENGTH 30
#define ACTIVATE_THRESHOLD 50 // threshold to remove cable noise
#define FREQ_IT 3
#define FADEOUT_CONST 245
#define COLOR_MAX 255

// define LED pins on the shield
int LED[] = {3, 5, 6, 9, 10, 11};
Adafruit_NeoPixel strips[NUM_STRIPS];

//Define spectrum variables
int freqAmp;
int freqOne[NUM_STRIPS];
int freqTwo[NUM_STRIPS]; 
int i;
int32_t freq;
int8_t c; // counter

#define DEBUG_MSG false

uint32_t lights[NUM_LEDS];

void setup()
{
  #if DEBUG_MSG
    Serial.begin(9600);
  #endif

  initSpectrumShield();
 
  turnOffPixels();
}

/*
 * configure output pins and spectrum shield input pins
 */
void initSpectrumShield() {
  //Set LED pin configurations
  for(i = 0; i < NUM_STRIPS; i++)
  {
    strips[i] = Adafruit_NeoPixel(NUM_LEDS, LED[i], NEO_RGBW + NEO_KHZ800);

    strips[i].begin();
    strips[i].setBrightness(40); // set brightness to n%
  }

  //Set Spectrum Shield pin configurations
  pinMode(STROBE, OUTPUT);
  pinMode(RESET, OUTPUT);
  pinMode(DC_ONE, INPUT);
  pinMode(DC_TWO, INPUT);  
  digitalWrite(STROBE, HIGH);
  digitalWrite(RESET, HIGH);
  
  //Initialize Spectrum Analyzers
  digitalWrite(STROBE, LOW);
  delay(1);
  digitalWrite(RESET, HIGH);
  delay(1);
  digitalWrite(STROBE, HIGH);
  delay(1);
  digitalWrite(STROBE, LOW);
  delay(1);
  digitalWrite(RESET, LOW);  

  Serial.begin(9600);
}

void loop()
{
  readFrequencies();
  showColor();
  delay(5);
}


/*
 * pull frequencies from spectrum shield
 */
void readFrequencies(){
  // read frequencies for each band
  for (freqAmp = 0; freqAmp < NUM_STRIPS; freqAmp++)
  {
    digitalWrite(STROBE, HIGH);
    delayMicroseconds(50);
    digitalWrite(STROBE, LOW);
    delayMicroseconds(50);

    freqOne[freqAmp] = analogRead(DC_ONE);
    freqTwo[freqAmp] = analogRead(DC_TWO); 
  }
}

/*
 * light LEDs based on frequencies
 * 
 * sum up frequencies to smooth output
 */
void showColor() {
  c++;

  for(int i = 5; i < NUM_STRIPS; i++) {
    if(freqTwo[i] > freqOne[i]){
      freq +=  freqTwo[i]/4;
    } else{
      freq +=  freqOne[i]/4;
    }
    
    // set color every n-th iteration (n = FREQ_IT)
    // otherwise fade out pixels
    if (c == FREQ_IT) {
      freq /= FREQ_IT;
      turnOnPixels(i, calculateNumOfPixels(freq));

      c = 0;
      freq = 0;
    }

    fadeOutPixels(i, calculateNumOfPixels(freq));
  }
}

/*
 * calculate the number of LEDs that should be lit based on current frequency value
 */
uint32_t calculateNumOfPixels(int32_t freq) {
 return max(0, (freq - ACTIVATE_THRESHOLD)) * NUM_LEDS / 255;
}

/*
 * sets strip color to 000 (off)
 */
void turnOffPixels()
{
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      strips[i].setPixelColor(j, 0, 0, 0);
    }
    strips[i].show();
  }
}

/*
 * set each pixel a bit darker
 */
void fadeOutPixels(uint32_t i, uint32_t numOfPixels) {
    uint32_t color;
    uint8_t r, g, b;

    for (int j = numOfPixels; j < NUM_LEDS; j++) {
      color = strips[i].getPixelColor(j);

      b = color >> 16;
      r = color >> 8;
      g = color;
      strips[i].setPixelColor(j, strips[i].Color(g * FADEOUT_CONST / COLOR_MAX, r * FADEOUT_CONST / COLOR_MAX, b * FADEOUT_CONST / COLOR_MAX));
  }
}

/*
 * sets strip color based on lights array
 * the brightness is determined by the position of the LED in a wave
 * 
 * edges are darker than the middle
 * this "smoothes" the wave effect
 */
void turnOnPixels(uint32_t i, uint32_t numOfPixels)
{   
  #if DEBUG_MSG
    Serial.println(numOfPixels);
  #endif

  for (int j = 0; j < numOfPixels; j++) {
    strips[i].setPixelColor(j, strips[i].Color(0, 100, 0));
  }

  strips[i].show();
}
