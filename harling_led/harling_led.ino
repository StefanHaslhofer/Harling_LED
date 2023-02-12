#include <Adafruit_NeoPixel.h>

// declare spectrum shield pin connections
#define STROBE 4
#define RESET 6
#define DC_ONE A0
#define DC_TWO A1 
#define NUM_STRIPS 6
#define NUM_STRIPS 6

// define LED pins on the shield
int LED[] = {3, 5, 6, 9, 10, 11};
Adafruit_NeoPixel strips[NUM_STRIPS];

//Define spectrum variables
int freqAmp;
int freqOne[NUM_STRIPS];
int freqTwo[NUM_STRIPS]; 
int i;

#define NUM_LEDS 30
#define WAVE_LENGTH 30
#define COLOR_MAX 255
#define MAX_DELAY 150

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
    strips[i].setBrightness(20); // set brightness to n%
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
  delay(50);
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
 */
void showColor(){
  for(int i= 5; i<NUM_STRIPS; i++) {
    if(freqTwo[i] > freqOne[i]){
      turnOnPixels(5, freqTwo[i]/4);
    } else{
      turnOnPixels(5, freqOne[i]/4);
    }
  }
}

/*
 * calculate color of current LED strip
 */
uint32_t calculateColor(uint32_t color, uint32_t volume) {
 
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
 * sets strip color based on lights array
 * the brightness is determined by the position of the LED in a wave
 * 
 * edges are darker than the middle
 * this "smoothes" the wave effect
 */
void turnOnPixels(uint32_t i, uint32_t freq)
{   
  int numOfPixels = (freq - 50) * NUM_LEDS / 255;

  for (int j = 0; j < numOfPixels; j++) {
    strips[i].setPixelColor(j, strips[i].Color(20, 150, 20));
  }

  for (int j = numOfPixels; j < NUM_LEDS; j++) {
    strips[i].setPixelColor(j, strips[i].Color(0, 0, 0));
  }
  strips[i].show();
}
