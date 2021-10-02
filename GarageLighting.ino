#include <ClickEncoder.h>
#include <TimerOne.h>
#include <FastLED.h>
#include "fix_fft.h"

int16_t oldEncPos, encPos, oldEncPos2, encPos2;
uint8_t buttonState, buttonState2;
bool colorOrFrequency;
int freqIndex = 1;
uint8_t beatspermin = 60;
int dataTemp;
int index = 0;
int dat[25];
bool cycleMode = false;

#define pinA 46 // frequency and color control
#define pinB 44 // frequency and color control
#define pinSw 42 // mode
#define STEPS 4
#define NUM_LEDS 59 //moon shelf
#define NUM_LEDS2  60 //window
#define NUM_LEDS3 59  //headboard
#define NUM_LEDS4 59 //bottom of desk
#define closPin 10 // moon shelf
#define winPin 9 //desk
#define backPin 8 //headboard
#define bottPin 2 //bedframe
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define FRAMES_PER_SECOND  120



int BRIGHTNESS = 254;

#define FRAMES_PER_SECOND  120


ClickEncoder encoder(pinA, pinB, pinSw, STEPS);

CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];
CRGB leds3[NUM_LEDS3];
CRGB leds4[NUM_LEDS4];

//DEFINE Palettes


const CRGBPalette16 clubStriped = CRGBPalette16(
    CRGB::Red,  CRGB::Black,  CRGB::Purple, CRGB::Black,
    CRGB::Red,  CRGB::Black,  CRGB::Blue,   CRGB::Black,
    CRGB::Red,  CRGB::Black,  CRGB::Purple, CRGB::Black,
    CRGB::Red,  CRGB::Black,  CRGB::Blue,   CRGB::Black
);

const CRGBPalette16 partyStriped = CRGBPalette16(
    CRGB::Red,  CRGB::Black,  CRGB::Purple, CRGB::Black,
    CRGB::Orange, CRGB::Black,  CRGB::Indigo, CRGB::Black,
    CRGB::Yellow, CRGB::Black,  CRGB::Blue,   CRGB::Black,
    CRGB::Red,    CRGB::Black,  CRGB::Green,  CRGB::Black
);

const CRGBPalette16 lavaPalette = CRGBPalette16(
    CRGB::DarkRed,  CRGB::Orange, CRGB::DarkRed,  CRGB::Red,
    CRGB::Black,  CRGB::Maroon, CRGB::DarkRed,  CRGB::DarkRed,
    CRGB::Black,  CRGB::DarkRed, CRGB::Red,     CRGB::Orange,
    CRGB::White,    CRGB::Orange,  CRGB::Red,     CRGB::DarkRed
);

//const CRGBPalette16 greenRetroStripe = CRGBPalette16(
//    CRGB::Green,  CRGB::Black,  CRGB::Purple,   CRGB::Black,
//    CRGB::Green,  CRGB::Black,  CRGB::Orange,     CRGB::Black,
//    CRGB::Green,  CRGB::Black,  CRGB::Purple,   CRGB::Black,
//    CRGB::Green,  CRGB::Black,  CRGB::Orange,     CRGB::Black
//);

const CRGBPalette16 tronLegacyStripe = CRGBPalette16(
    CRGB::Cyan,   CRGB::Black,  CRGB::OrangeRed,   CRGB::Black,
    CRGB::Cyan,   CRGB::Black,  CRGB::Red,   CRGB::Black,
    CRGB::Cyan,   CRGB::Black,  CRGB::OrangeRed,   CRGB::Black,
    CRGB::Cyan,   CRGB::Black,  CRGB::Red,   CRGB::Black
);

const CRGBPalette16 sunsetStripe = CRGBPalette16(
    CRGB::Red, CRGB::Black,  CRGB::Gold,  CRGB::Black,
    CRGB::Orange, CRGB::Black,  CRGB::Red, CRGB::Black,
    CRGB::Red, CRGB::Black,  CRGB::DarkOrange,   CRGB::Black,
    CRGB::Orange, CRGB::Black,  CRGB::Red,  CRGB::Black
);
    
    

const CRGBPalette16 oceanColors = CRGBPalette16(
    CRGB::MidnightBlue, CRGB::DarkBlue, CRGB::MidnightBlue, CRGB::Navy,
    CRGB::DarkBlue,   CRGB::MediumBlue, CRGB::SeaGreen,     CRGB::Teal,
    CRGB::CadetBlue,  CRGB::Blue,       CRGB::DarkCyan,     CRGB::CornflowerBlue,
    CRGB::Aquamarine, CRGB::SeaGreen,   CRGB::Aqua,         CRGB::LightSkyBlue
);

const CRGBPalette16 coldColorsOnly = CRGBPalette16(
    CRGB::MidnightBlue, CRGB::Purple, CRGB::DarkRed, CRGB::Aqua,
    CRGB::DarkBlue,   CRGB::MediumBlue, CRGB::Maroon,     CRGB::Navy,
    CRGB::CadetBlue,  CRGB::DarkSalmon, CRGB::DarkCyan,     CRGB::Fuchsia,
    CRGB::Aquamarine, CRGB::SeaGreen,   CRGB::Aqua,         CRGB::Crimson
);

DEFINE_GRADIENT_PALETTE( fullClub_gp ){
  0,  255,  0,  113,  //pink
  48, 0,    0,  255,  //blue
  84, 0,   212, 255,  //cyan
  132, 255,  0,  0,    //red
  186, 125,  0,  255,  //Purple
  255,  255,  0,  113 
};
//
//DEFINE_GRADIENT_PALETTE( spooky_gp ){
//  0,  157,  0,  255,  //purple
// 33,  255,  97, 0,  //orange
// 59,  17, 162,  0,  //light green
// 83,  24, 92, 0,    //dark green
//  0,  157,  0,  255,  //purple
//};

const CRGBPalette16 filledClub = fullClub_gp;
//const CRGBPalette16 spooky = spooky_gp;
CRGBPalette16 partyCol = PartyColors_p;



typedef void (*SimplePatternList[])();
CRGBPalette16 palettes[] = { clubStriped, tronLegacyStripe, partyStriped, partyCol, sunsetStripe, lavaPalette, coldColorsOnly, RainbowColors_p};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t currentPalette = 0;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
int col = 0;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPalette(){
  currentPalette = (currentPalette + 1) % ARRAY_SIZE( palettes ) ;
}

void fadeAllBy(int fade){
  for(int i = 0; i < NUM_LEDS2; i++){
    if(i < NUM_LEDS)
      leds[i].fadeToBlackBy(fade);
    if(i < NUM_LEDS3)
      leds3[i].fadeToBlackBy(fade);
    if(i < NUM_LEDS4)
      leds4[i].fadeToBlackBy(fade);
    leds2[i].fadeToBlackBy(fade);
  }
}

int GetMicData(int ind){
  char im[128], data[128];
  int val;
  int min = 1024, max = 0;
  for(int i = 0 ; i < 128; i++){
    val = analogRead(A8);
    data[i] = val/4 - 128;
    im[i] = 0;
    if ( val > max) max = val;
    if(val < min)  min = val;
  }

  fix_fft(data, im, 7, 0);
  for(int i = 0; i < 25; i++){
    dat[i] = sqrt(data[i] * data[i] + im[i] *im[i]);
  }
  Serial.println(dat[ind]);
  return dat[ind];
}

void PullForward(bool forward, bool backAround = false){

  if(!forward){
      //pulls from end on line back to base
      for(int i = 0 ; i < NUM_LEDS2-1; i++){
        if(i < NUM_LEDS-1)
         leds[i] = leds[i+1];
    
        if(i < NUM_LEDS3-1)
          leds3[i]=leds3[i+1];
    
        if(i < NUM_LEDS4-1)
          leds4[i] = leds4[i+1];
   
        leds2[i]=leds2[i+1];
      }    
      if(backAround){
          leds[NUM_LEDS-1] = leds[0];
          leds2[NUM_LEDS2-1] = leds2[0];
          leds3[NUM_LEDS3-1] = leds3[0];
          leds4[NUM_LEDS4-1] = leds4[0];
      }
  }
  else{
      //pulls from base to end of line
      for(int i = NUM_LEDS2-1 ; i > 0; i--){
        if(i < NUM_LEDS)
         leds[i] = leds[i-1];
    
        if(i < NUM_LEDS3)
          leds3[i]=leds3[i-1];
    
        if(i < NUM_LEDS4)
          leds4[i] = leds4[i-1];
   
        leds2[i]=leds2[i-1];
      }
      if(backAround){
          leds[0] = leds[NUM_LEDS-1];
          leds2[0] = leds2[NUM_LEDS2-1];
          leds3[0] = leds3[NUM_LEDS3-1];
          leds4[0] = leds4[NUM_LEDS4-1];
      }
    
  }    
}

void freqAnalyzer(){
  int d = GetMicData(freqIndex);
  
  for(int i = 0 ; i < NUM_LEDS2; i++){
    if(i < NUM_LEDS){
      if(i < (dat[freqIndex] * 5)-10)
        leds[i] = ColorFromPalette(palettes[currentPalette],gHue, BRIGHTNESS, LINEARBLEND);
      else
        leds[i].fadeToBlackBy(37);
    }
    if(i < NUM_LEDS3){
      if(i < (dat[freqIndex] * 8)-10)
        leds3[i] = ColorFromPalette(palettes[currentPalette],gHue+120, BRIGHTNESS, LINEARBLEND);
      else
        leds3[i].fadeToBlackBy(37);
    }
   if( dat[freqIndex] > 23&& i < NUM_LEDS4){
      leds4[i] = ColorFromPalette(palettes[currentPalette],gHue, BRIGHTNESS, LINEARBLEND);
    }
   else
      leds4[i].fadeToBlackBy(37);
  if( dat[freqIndex] > 15){
    leds2[i] = ColorFromPalette(palettes[currentPalette],gHue+120, BRIGHTNESS, LINEARBLEND);
  }
  else
     leds2[i].fadeToBlackBy(42);
  }
}

void freqAnalyzerRainbow(){
  int d = GetMicData(freqIndex);
  for(int i = 0 ; i < NUM_LEDS2; i++){
    if(i < NUM_LEDS){
      if(i < (dat[freqIndex] * 5)-10)
        leds[i] = ColorFromPalette(palettes[currentPalette],gHue+i, BRIGHTNESS, LINEARBLEND);
      else
        leds[i].fadeToBlackBy(37);
    }
    if(i < NUM_LEDS3){
      if(i < (dat[freqIndex] * 8)-10)
        leds3[i] = ColorFromPalette(palettes[currentPalette],gHue+120+i, BRIGHTNESS, LINEARBLEND);
      else
        leds3[i].fadeToBlackBy(37);
    }
   if( dat[freqIndex] > 23&& i < NUM_LEDS4){
      leds4[i] = ColorFromPalette(palettes[currentPalette],gHue+i, BRIGHTNESS, LINEARBLEND);
    }
   else
      leds4[i].fadeToBlackBy(37);
  if( dat[freqIndex] > 15){
    leds2[i] = ColorFromPalette(palettes[currentPalette],gHue+120+i, BRIGHTNESS, LINEARBLEND);
  }
  else
     leds2[i].fadeToBlackBy(42);
  }
}

void freqWaterfall(){
  char im[128], data[128];
  int val;
  int min = 1024, max = 0;
  for(int i = 0 ; i < 128; i++){
    val = analogRead(A8);
    data[i] = val/4 - 128;
    im[i] = 0;
    if ( val > max) max = val;
    if(val < min)  min = val;
  }

  fix_fft(data, im, 7, 0);
  int dat[25];
  for(int i = 0; i < 25; i++){
    dat[i] = sqrt(data[i] * data[i] + im[i] *im[i]);
  }
  Serial.println(dat[freqIndex]);
  if(dat[freqIndex] > 20)
        leds[0] = ColorFromPalette(palettes[currentPalette],gHue++, BRIGHTNESS, LINEARBLEND);
  else
        leds[0] = CRGB::Black;
  if(dat[freqIndex] > 20)
        leds3[0] = ColorFromPalette(palettes[currentPalette],gHue++, BRIGHTNESS, LINEARBLEND);
  else
        leds3[0] = CRGB::Black;
  if(data[freqIndex] > 20)
        leds4[0] = ColorFromPalette(palettes[currentPalette],gHue, BRIGHTNESS, LINEARBLEND);
  else
        leds4[0] = CRGB::Black;
  if( dat[freqIndex] > 20)
        leds2[0] = ColorFromPalette(palettes[currentPalette],gHue, BRIGHTNESS, LINEARBLEND);
  else
        leds2[0] = CRGB::Black;
  for(int i = NUM_LEDS2-1 ; i > 0; i--){
    if(i < NUM_LEDS)
      leds[i] = leds[i-1];
    
    if(i < NUM_LEDS3)
      leds3[i]=leds3[i-1];
    
   if(i < NUM_LEDS4)
      leds4[i] = leds4[i-1];
   
  leds2[i]=leds2[i-1];
  }
}

void freqMeter(){
    char im[128], data[128];
  int val;
  int min = 1024, max = 0;
  for(int i = 0 ; i < 128; i++){
    val = analogRead(A8);
    data[i] = val/4 - 128;
    im[i] = 0;
    if ( val > max) max = val;
    if(val < min)  min = val;
  }

  fix_fft(data, im, 7, 0);
  int dat[25];
  for(int i = 0; i < 25; i++){
    dat[i] = sqrt(data[i] * data[i] + im[i] *im[i]);
  }
  Serial.println(dat[freqIndex]);
  if(dat[freqIndex] > 30)
    gHue+=3;
  for(int i = 0; i < NUM_LEDS2; i++){
    if(dat[freqIndex] * 2 > i && dat[freqIndex] > 1)
      leds2[i] = ColorFromPalette(palettes[currentPalette],gHue+i, BRIGHTNESS, LINEARBLEND);
    else
      leds2[i].fadeToBlackBy(32);
    if(dat[freqIndex] * 2 > i && dat[freqIndex] > 1 && i < NUM_LEDS){
      leds[i] = ColorFromPalette(palettes[currentPalette],gHue+i, BRIGHTNESS, LINEARBLEND);
    }
    else if(i < NUM_LEDS){
      leds[i].fadeToBlackBy(32);
    }
    if(dat[freqIndex] * 2 > i && dat[freqIndex] > 1 && i < NUM_LEDS3){
      leds3[i] = ColorFromPalette(palettes[currentPalette],gHue+i, BRIGHTNESS, LINEARBLEND);
    }
    else if(i < NUM_LEDS3){
      leds3[i].fadeToBlackBy(32);
    }
  }
}

void fruityPebbles(){
  int d = GetMicData(freqIndex);
  int randInd;
  if(d < 7)
    d = 0;
  d = d/3;
  fadeAllBy(50);
  for(int i = 0; i < d; i++){
    randInd = random(0, NUM_LEDS2);
    if(randInd < NUM_LEDS){
      leds[randInd] = ColorFromPalette(palettes[currentPalette], random(255), BRIGHTNESS, LINEARBLEND);
    }
    if(randInd < NUM_LEDS3){
      leds3[randInd] = ColorFromPalette(palettes[currentPalette], random(255), BRIGHTNESS, LINEARBLEND);
    }
    if(randInd < NUM_LEDS4){
      leds4[randInd] = ColorFromPalette(palettes[currentPalette], random(255), BRIGHTNESS, LINEARBLEND);
    }
    leds2[randInd] = ColorFromPalette(palettes[currentPalette], random(255), BRIGHTNESS, LINEARBLEND);
  }
}

void chasingTail(){

  int da = GetMicData(freqIndex)/4 -1 ;
  fadeAllBy(24);
  for(int i = 0; i < da; i++){
    index++;
    if(index > 32000)
      index = 0;
    leds[index%NUM_LEDS] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
    leds[NUM_LEDS-(index%NUM_LEDS)-1] += ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
    leds3[index%NUM_LEDS3] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
    leds3[NUM_LEDS3-(index%NUM_LEDS3)-1] += ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
    leds2[index%NUM_LEDS2] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
    leds2[NUM_LEDS2-(index%NUM_LEDS2) - 1] += ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
    leds4[index%NUM_LEDS4] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
    leds4[NUM_LEDS4-(index%NUM_LEDS4) - 1] += ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
  }
  
}
bool first = true;
bool swit = true;

void alternateWub(){
  if(first){
    fill_palette(leds, NUM_LEDS, 10, 1, palettes[currentPalette], 255, NOBLEND);
    fill_palette(leds2, NUM_LEDS2, 10, 1, palettes[currentPalette], 255, NOBLEND);
    fill_palette(leds3, NUM_LEDS3, 10, 1, palettes[currentPalette], 255, NOBLEND);
    fill_palette(leds4, NUM_LEDS4, 10, 1, palettes[currentPalette], 255, NOBLEND);
    first = false;
  }
  int d = GetMicData(freqIndex);
  PullForward(swit, true);
  if(d > 16 && !(dataTemp > 16))
        swit=!swit;
  dataTemp=d;
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, BRIGHTNESS);
      fadeToBlackBy( leds2, NUM_LEDS2, 20);
  int pos2 = beatsin16( 13, 0, NUM_LEDS2-1 );
  leds2[pos2] += CHSV( gHue, 255, BRIGHTNESS);
      fadeToBlackBy( leds3, NUM_LEDS3, 20);
  int pos3 = beatsin16( 13, 0, NUM_LEDS3-1 );
  leds3[pos3] += CHSV( gHue, 255, BRIGHTNESS);
        fadeToBlackBy( leds4, NUM_LEDS4, 20);
  int pos4 = beatsin16( 13, 0, NUM_LEDS4-1 );
  leds4[pos4] += CHSV( gHue, 255, BRIGHTNESS);
}

int indi = 0;


void clubDelta(){
  fadeAllBy(20);
  int pos = beatsin16( 60, 0, NUM_LEDS-1 );
  if(indi < 118)
    leds[pos] += ColorFromPalette(palettes[currentPalette], gHue+indi, BRIGHTNESS, LINEARBLEND);
  else if(indi < 238)
    leds2[pos] += ColorFromPalette(palettes[currentPalette], gHue+indi, BRIGHTNESS, LINEARBLEND);
  else
    leds3[pos] += ColorFromPalette(palettes[currentPalette], gHue+indi, BRIGHTNESS, LINEARBLEND);
  indi++;
  if(indi > 356){
    indi = 0;
    nextPalette();
  }
}

int bar = 0;

void randomBars(){
  delay(10);
  fadeAllBy(60);
  int barLoc = random(5,54);
  bar = random(0,3);
  if(bar == 0){
    for(int i = 0; i < 6; i++){
      leds[barLoc - i] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
      leds[barLoc + i] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
    }
  }
  if(bar == 1){
    for(int i = 0; i < 6; i++){
      leds2[barLoc - i] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
      leds2[barLoc + i] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
    }
  }
  if(bar == 2){
    for(int i = 0; i < 6; i++){
      leds3[barLoc - i] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
      leds3[barLoc + i] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
    }
  }
  
  
}





void rotateIt(){
  delay(100);
  fadeAllBy(255);
  if(bar == 0){
    fill_palette( leds, NUM_LEDS, gHue, 255/NUM_LEDS, palettes[currentPalette], BRIGHTNESS, LINEARBLEND);
    bar++;
  }       
  else if(bar == 1){

    fill_palette( leds2, NUM_LEDS2, gHue, 255/NUM_LEDS, palettes[currentPalette], BRIGHTNESS, LINEARBLEND);
    bar++;
  } 
  else if(bar == 2){

    fill_palette( leds3, NUM_LEDS3, gHue, 255/NUM_LEDS, palettes[currentPalette], BRIGHTNESS, LINEARBLEND);
    bar = 0;
    nextPalette();
  }        
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_palette( leds, NUM_LEDS, gHue, 255/NUM_LEDS, palettes[currentPalette], BRIGHTNESS, LINEARBLEND);
  fill_palette( leds2, NUM_LEDS2, gHue, 255/NUM_LEDS2, palettes[currentPalette], BRIGHTNESS, LINEARBLEND);
  fill_palette( leds3, NUM_LEDS3, gHue, 255/NUM_LEDS3, palettes[currentPalette], BRIGHTNESS, LINEARBLEND);
  fill_palette( leds4, NUM_LEDS4, gHue, 255/NUM_LEDS4, palettes[currentPalette], BRIGHTNESS, LINEARBLEND);

}

int sent = 0;
int warpWindup = 30;
int leftover = 40;

bool backward = false;

void warpSpeed(){
  sent++;
  PullForward(!backward);
  if(sent < 6){
      leds[0] = ColorFromPalette(palettes[currentPalette], gHue++, BRIGHTNESS, LINEARBLEND);
      leds2[0] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
      leds3[0] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
  }
  else if(sent < 20)
  {
    leds[0] = CRGB::Black;
    leds2[0] = CRGB::Black;
    leds3[0] = CRGB::Black;
  }
  else{
    sent = 0;
    if(warpWindup > 0)
      warpWindup--;
    else if(leftover > 0){
      leftover--;
    }
    else{
      warpWindup = 30;
      leftover = 60;
    }
  }
    
  delay(warpWindup);
}

void quickWitted(){
  sent++;
  PullForward(!backward);
  if(sent < 6){
      leds[0] = ColorFromPalette(palettes[currentPalette], gHue++, BRIGHTNESS, LINEARBLEND);
  }
  else
      leds[0] = CRGB::Black;
      
  if(sent > 7 && sent < 13)
  {
      leds2[0] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
  }
  else
      leds2[0] = CRGB::Black;

  if(sent > 14 && sent < 20)
      leds3[0] = ColorFromPalette(palettes[currentPalette], gHue, BRIGHTNESS, LINEARBLEND);
  else
      leds3[0] = CRGB::Black;
      
  if(sent > 30)
    sent = 0; 
}

int onTheRadius = 0;

void spiral(){
  if(onTheRadius > 58){
    onTheRadius = 0;
    nextPalette();
    fadeAllBy(255);
  }
  if(onTheRadius % 3 == 0){
    leds[onTheRadius] = ColorFromPalette(palettes[currentPalette], gHue++, BRIGHTNESS, LINEARBLEND);
  }
  else if(onTheRadius % 3 == 1){
    leds2[onTheRadius] = ColorFromPalette(palettes[currentPalette], gHue++, BRIGHTNESS, LINEARBLEND);
  }
  else if(onTheRadius % 3 == 2){
    leds3[onTheRadius] = ColorFromPalette(palettes[currentPalette], gHue++, BRIGHTNESS, LINEARBLEND);
  }
  onTheRadius++;
   
}

void beatSwitch(){
  int d = GetMicData(freqIndex);
  fadeAllBy(30);
  if(d > 25){
      if(bar == 0){
        for(int i = 0; i < NUM_LEDS3; i++){
          leds3[i] = ColorFromPalette(palettes[currentPalette], gHue+i, BRIGHTNESS, LINEARBLEND);
        }
        bar++;
      }       
      else if(bar == 1){
        for(int i = 0; i < NUM_LEDS2; i++){
          leds2[i] = ColorFromPalette(palettes[currentPalette], gHue+i, BRIGHTNESS, LINEARBLEND);
        }
        bar++;
      }    
      else if(bar == 2){
        for(int i = 0; i < NUM_LEDS; i++){
          leds[i] = ColorFromPalette(palettes[currentPalette], gHue+i, BRIGHTNESS, LINEARBLEND);
        }
        bar = 0;
      }            
  }

}

void turnOff(){
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
     for( int i = 0; i < NUM_LEDS2; i++) { //9948
    leds2[i] = CRGB::Black;
  }
  for( int i = 0; i < NUM_LEDS3; i++) { //9948
    leds3[i] = CRGB::Black;
  }
    for( int i = 0; i < NUM_LEDS4; i++) { //9948
    leds4[i] = CRGB::Black;
  }
  FastLED.show();
}

SimplePatternList gPatterns = { spiral,  warpSpeed, beatSwitch, quickWitted, chasingTail, randomBars, fruityPebbles, rotateIt,  freqMeter, clubDelta, freqWaterfall};

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}



void setup() {
  delay(6000);
  Serial.begin(115200);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
  randomSeed(analogRead(A0));

  //encoder.setAccelerationEnabled(true);
  //encoder2.setAccelerationEnabled(true;


  FastLED.addLeds<LED_TYPE,closPin,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,winPin,COLOR_ORDER>(leds2, NUM_LEDS2).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,backPin,COLOR_ORDER>(leds3,NUM_LEDS3).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,bottPin,COLOR_ORDER>(leds4,NUM_LEDS4).setCorrection(TypicalLEDStrip);
  oldEncPos = -1;
  oldEncPos2 = -1;
  encoder.setAccelerationEnabled(0);
  colorOrFrequency = false;

}

int offState = 0;
int bright = 0;

void loop() {
  encPos += encoder.getValue();
  if(gCurrentPatternNumber < 3)
    colorOrFrequency = true;
  else
    colorOrFrequency = false;
 

if(encPos != oldEncPos){
    nextPalette();
    first = true;
}
  

  oldEncPos = encPos;

  FastLED.setBrightness(BRIGHTNESS);
  buttonState = encoder.getButton();
  if (buttonState != 0) {
    switch (buttonState) {
      case ClickEncoder::Open:          //0
        break;

      case ClickEncoder::Closed:        //1
        break;

      case ClickEncoder::Pressed:       //2
        break;

      case ClickEncoder::Held:          //3
        break;

      case ClickEncoder::Released:      //4
        break;

      case ClickEncoder::Clicked:       //5
        break;

      case ClickEncoder::DoubleClicked: //6
        break;
    }
  }

    // Call the current pattern function once, updating the 'leds' array
  if(offState == 0){
    gPatterns[gCurrentPatternNumber]();
  }
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  //FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  if(bright > 0){
    if(bright == 1){
      EVERY_N_SECONDS(12) { nextPalette(); first = true; }
    }
    else if(bright == 2){
      EVERY_N_SECONDS(12) { nextPalette(); first = true; }
      EVERY_N_SECONDS(40) { nextPattern(); }
    }
    else
      bright = 0;
  }
  if(buttonState == 5){
    nextPattern();
    first = true;
    offState = 0;
  }
  if(buttonState == 3||offState==1){
    turnOff();
    offState = 1;
  }
  if(buttonState==6){
    bright++;
  }
}


void timerIsr() {
  encoder.service();
}
