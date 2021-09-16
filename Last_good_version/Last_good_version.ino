#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>

// How many leds are in the strip?
#define NUM_LEDS 140
// Data pin that led data will be written out over
#define DATA_PIN 2
#define BRIGHTNESS 80

// Horloge
#define DS1307_ADDRESS 0x68
byte zero = 0x00; // hack fix


// Config setup
int ledMatrix[16][9] = {
    {1000,1000,1000, 47, 48, 79, 1000, 1000, 1000},
    {1000,1000,18 ,46 ,49 ,78  ,80 ,1000,1000},
    {1000,17 ,19  ,45 ,50 ,77 ,81  ,105 ,1000},
    {1000, 16, 20, 44, 51, 76, 82, 104, 1000},
    {1000, 15, 21, 43, 52, 75, 83, 103, 1000},
    {0, 14, 22, 42, 53, 74, 84, 102, 106},
    {1, 13, 23, 41, 54, 73, 85, 101, 107},
    {2, 12, 24, 40, 55, 72, 86, 100, 108},
    {3, 11, 25, 39, 56, 71, 87, 99, 109},
    {4, 10, 26, 38, 57, 70, 88, 98, 110},
    {5, 9, 27, 37, 58, 69, 89, 97, 111},
    {1000, 8, 28, 36, 59, 68, 90, 96, 1000},
    {1000, 7, 29, 35, 60, 67, 91, 95, 1000},
    {1000, 6, 30, 34, 61, 66, 92, 94, 1000},
    {1000, 1000, 31, 33, 62, 65, 93, 1000, 1000},
    {1000, 1000, 1000, 32, 63, 64, 1000, 1000, 1000}};


// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];
int limit = 2;

// color2 : violet vert
CRGB colorRGB = CRGB(238, 100, 238);
CRGB colorRGB2 = CRGB(0, 180, 20);


// This function sets up the leds and tells the controller about them
void setup() {

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    Wire.begin();
    setDateTime();
    
  
    //sanity check delay - allows reprogramming if accidently blowing power w/leds
    delay(2000);
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    Serial.begin(9600);  
}

char Time[] = "  :  :  ";
char Calendar[] = "  /  /20  ";
char temperature[] = " 00.00";
char temperature_msb;
byte i, second, minute, hour, day, date, month, year, temperature_lsb;

// start test date
void setDateTime()
{
  byte second = 00;   //0-59
  byte minute = 32;   //0-59
  byte hour = 17;     //0-23
  byte weekDay = 4;  //1-7
  byte monthDay= 16; //1-31
  byte month = 9;     //1-12
  byte year = 21;     //0-99
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.write(zero); //start
  Wire.endTransmission();
}

byte decToBcd(byte val)
{
  // Convert normal decimal numbers to binary coded decimal
  return ((val / 10 * 16) + (val % 10));
}

byte bcdToDec(byte val)
{
  // Convert binary coded decimal to normal decimal numbers
  return ((val / 16 * 10) + (val % 16));
}

void fillLine(int index, CRGB color)
{
  for (int i = 0; i < 9; i++)
  {
    leds[ledMatrix[index - 1][i]] = color;
  }
}

void clearLine(int index)
{
  for (int i = 0; i < 9; i++)
  {
    leds[ledMatrix[index - 1][i]] = CHSV(0, 0, 0);
  }
}

void loop() {

  
  //Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 7);
  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read());         //0-6 -> sunday – Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());

  int division = minute / 3.75;
  int newLimit = division + 1;
  Serial.print("time : ");
  Serial.print(hour);
  Serial.print(" : ");
  Serial.print(minute);
  Serial.print(" : ");
  Serial.print(second);
  Serial.println(limit);
  delay(990);
   // Main loop for leds
  for (int y = limit + 1; y <= 16; y++)
  {
    fillLine(y, colorRGB2);
  }
  for (int z = 0; z < limit; z++)
  {
    fillLine(z, colorRGB);
  }
  clearLine(limit);
  FastLED.show();
  limit = newLimit;
     
}
