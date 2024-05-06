/*
  Driver code to control the Adafruit P2477 LED Disk.
  The code is able to receive serial communication commands from a python script: RingLEDBoard.py.

  @author: Siyu (Steven) Lin
  Sep 11, 2023
*/
#include <Adafruit_DotStar.h>
#include <SPI.h>   

// Python Communication Code
const int HANDSHAKE     = 0;
const int LIGHTUPSINGLE = 1;
const int TURNOFFSINGLE = 2;
const int LIGHTUPRING   = 3;
const int TURNOFFRING   = 4;
const int LIGHTUPALL    = 5;
const int TURNOFFALL    = 6;
const int SETBRIGHTNESS = 7;


#define NUMPIXELS 255 // Number of LEDs on the board
#define NUMRINGS  10  // Number of rings of LEDs 
// Min and Max index of LEDs along each ring
int RingIdxMin[NUMRINGS] = {254,248,236,216,192,164,132, 92, 48,  0}; 
int RingIdxMax[NUMRINGS] = {254,253,247,235,215,191,163,131, 91, 47}; 


// Pin-Outs
#define DATAPIN    4
#define CLOCKPIN   5
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_GBR);

// Color for LED OFF
uint32_t color_blank = strip.Color(0, 0, 0);

// Buffer to store communication data
byte buf[10];

int IDX;
int R;
int G;
int B;
int brightness = 5;

void setup() {
  strip.begin();
  strip.setBrightness(brightness);
  Serial.begin(115200);
}

void loop(){
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    switch(inByte) {
      case HANDSHAKE:
        if (Serial.availableForWrite()) {
          Serial.println("Arduino connection established successfully.");
        }
        break;

      case LIGHTUPSINGLE:
        Serial.readBytesUntil('x', buf, 4);
        IDX = int(buf[0]); R = int(buf[1]); G = int(buf[2]); B = int(buf[3]);
        strip.setPixelColor(IDX, R, B, G);
        strip.show();
        break;

      case TURNOFFSINGLE:
        Serial.readBytesUntil('x', buf, 1);
        IDX = int(buf[0]);
        strip.setPixelColor(IDX, color_blank);
        strip.show();
        break;

      case LIGHTUPRING:
        Serial.readBytesUntil('x', buf, 4);
        IDX = int(buf[0]); R = int(buf[1]); G = int(buf[2]); B = int(buf[3]);
        for(int i = RingIdxMin[IDX]; i <= RingIdxMax[IDX]; i++ ){
          strip.setPixelColor(i, R, B, G);
        }
        strip.show();
        break;

      case TURNOFFRING:
        Serial.readBytesUntil('x', buf, 1);
        IDX = int(buf[0]);
        for(int i = RingIdxMin[IDX]; i <= RingIdxMax[IDX]; i++ ){
          strip.setPixelColor(i, color_blank);
        }
        strip.show();
        break;
      
      case LIGHTUPALL:
        Serial.readBytesUntil('x', buf, 4);
        R = int(buf[0]); G = int(buf[1]); B = int(buf[2]);
        for(int i = 0; i <= NUMPIXELS; i++ ){
          strip.setPixelColor(i, R, B, G);
        }
        strip.show();
        break;

      case TURNOFFALL:
        Serial.readBytesUntil('x', buf, 1);
        for(int i = 0; i <= NUMPIXELS; i++ ){
          strip.setPixelColor(i, color_blank);
        }
        strip.show();
        break;
      
      case SETBRIGHTNESS:
        Serial.readBytesUntil('x', buf, 1);
        brightness = int(buf[0]);
        strip.setBrightness(brightness);
        strip.show();
        break;
    }
  }
}