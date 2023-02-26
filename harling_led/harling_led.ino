#include <Adafruit_NeoPixel.h>

// declare spectrum shield pin connections
#define STROBE 4
#define RESET 6
#define DC_ONE A0
#define DC_TWO A1 

#define NUM_STRIPS 6
#define NUM_LEDS 30
#define ACTIVATE_THRESHOLD 50 // threshold to remove cable noise
#define PIXEL_UPDATE_IT 5
#define FADEOUT_CONST 245
#define COLOR_MAX 255
#define PIXEL_STEP_WIDTH 3

// define LED pins on the shield
int LED[] = {3, 5, 6, 9, 10, 11};
Adafruit_NeoPixel strips[NUM_STRIPS];

// define colors for led strips
int colors[NUM_STRIPS][3] = {{255, 0, 0}, {255, 100, 0}, {80, 255, 0}, {0, 255, 100}, {0, 0, 255}, {100, 0, 255}};

//Define spectrum variables
int freqAmp;
int freqOne[NUM_STRIPS];
int freqTwo[NUM_STRIPS]; 
int i;

int32_t freq[NUM_STRIPS];
int8_t c;

#define DEBUG_MSG false

void setup()
{
  #if DEBUG_MSG
    Serial.begin(9600);
  #endif

  initSpectrumShield();
  turnOffStrips();
}

/*
 * configure output pins and spectrum shield input pins
 */
void initSpectrumShield() {
  //Set LED pin configurations
  for(i = 0; i < NUM_STRIPS; i++)
  {
    strips[i] = Adafruit_NeoPixel(NUM_LEDS, LED[i], NEO_GRBW + NEO_KHZ800);

    strips[i].begin();
    strips[i].setBrightness(30); // set brightness to n%
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
  c++;
  readFrequencies();

  // update pixels every n-th iteration (n = PIXEL_UPDATE_IT)
  // otherwise fade out
  if (c == PIXEL_UPDATE_IT) {
    updatePixels(true, c);

    c = 0;
    // reset array
    memset(freq, 0, sizeof(freq));
  } else {
    updatePixels(false, c);
  }

  delay(15);
}

/*
 * pull frequencies from spectrum shield
 */
void readFrequencies(){
  // read frequencies for each band
  for (freqAmp = 0; freqAmp < NUM_STRIPS; freqAmp++)
  {
    digitalWrite(STROBE, HIGH);
    delayMicroseconds(20);
    digitalWrite(STROBE, LOW);
    delayMicroseconds(20);

    freqOne[freqAmp] = analogRead(DC_ONE);
    freqTwo[freqAmp] = analogRead(DC_TWO); 
  }
}

/*
 * light LEDs based on frequencies
 * 
 * sum up frequencies to smooth output
 */
void updatePixels(bool fadeIn, int8_t iter) {
  for(int i = 0; i < NUM_STRIPS; i++) {
    if(freqTwo[i] > freqOne[i]){
      freq[i] +=  freqTwo[i]/4;
    } else{
      freq[i] +=  freqOne[i]/4;
    }

    if(fadeIn) { 
      // calculate arithmetic middle of frequency values 
      turnOnPixels(i, calculateNumOfPixels(freq[i]/PIXEL_UPDATE_IT));
    } else {
      fadeOutPixels(i, calculateNumOfPixels(freq[i]/iter));
    }
  }
}

/*
 * calculate the number of LEDs that should be lit based on current frequency value
 */
uint32_t calculateNumOfPixels(int32_t freq) {
  return max(0, (freq - ACTIVATE_THRESHOLD)) * NUM_LEDS / 255;
}

/*
 * turn off pixels after a given index
 * this prevents the LEDs from flickering too much
 *
 * i: index of strip
 * numOfPixels: amount of pixels that should stay lit
 */
void turnOffPixels(uint32_t i, uint32_t numOfPixels) {
    uint32_t color;
    uint8_t r, g, b;

    // start the iteration with a given index (numOfPixels)
    // otherwise the whole strip is turned on and off which results in flickering
    for (int j = numOfPixels; j < NUM_LEDS; j++) {
      strips[i].setPixelColor(j, strips[i].Color(0, 0, 0));
    }
}

/*
 * set pixel color of strip based on color array
 * 
 * i: index of strip
 * numOfPixels: amount of pixels that should be lit
 */
void turnOnPixels(uint32_t i, uint32_t numOfPixels)
{   
  #if DEBUG_MSG
    Serial.print(i);
    Serial.print(":");
    Serial.println(numOfPixels);
  #endif

  for (int j = 0; j < numOfPixels; j++) {
    // turn on pixels stepwise to reduce flickering
    if(j % PIXEL_STEP_WIDTH == 0) {
      for(int n = j; n < j + PIXEL_STEP_WIDTH; n++) {
        strips[i].setPixelColor(n, 100, 0, 255);
      }
    }
  }

  strips[i].show();
}

/*
 * sets strip color to 000 (off)
 */
void turnOffStrips()
{
  for (int i = 0; i < NUM_STRIPS; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      strips[i].setPixelColor(j, 0, 0, 0);
    }
    strips[i].show();
  }
}