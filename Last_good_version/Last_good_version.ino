#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>

// How many leds are in the strip?
#define NUM_LEDS 160
// Data pin that led data will be written out over
#define DATA_PIN 2
#define BRIGHTNESS 50

// Else config
#define button1 9 // Button B1 is connected to Arduino pin 9
#define button2 8 // Button B2 is connected to Arduino pin 8

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
int limit = 2 ;

CHSVPalette16 test_pal ;


// color2 : violet vert
CHSV colorHSV1 = CHSV(238, 180, 60);
CHSV colorHSV2 = CHSV(120, 180, 60);
int colorIndex= 0;

CHSVPalette16 RainbowColorsHSV_p = (
  CHSV(HUE_RED, 180, 180),  
  CHSV(HUE_ORANGE, 180, 180),  
  CHSV(HUE_YELLOW, 180, 180),  
  CHSV(HUE_GREEN, 180, 180),  
  CHSV(HUE_AQUA, 180, 180),  
  CHSV(HUE_BLUE, 180, 180),  
  CHSV(HUE_PURPLE, 180, 180),  
  CHSV(HUE_PINK, 180, 180)
);
CHSVPalette16 currentPalette = RainbowColorsHSV_p;
uint8_t paletteIndex = 1;

uint8_t brightness = 100;


// Custom 
CHSV purple196 (196,180,brightness);
CHSV purple196_light (196,200,brightness);

CHSV jaune41 (41,180,brightness);
CHSV jaune41_light (41,180,brightness);

CHSV orange32 (32,180,brightness);
CHSV orange32_light (32,180,brightness);

CHSV vert102 (102,180,brightness);
CHSV vert102_light (102,150,brightness);

CHSV blue160 (150,180,brightness);
CHSV blue160_light (150,180,brightness);

CHSV jaune64 (64,180,brightness);
CHSV jaune64_light (64,180,brightness);

CHSV pink224 (224,180,brightness);
CHSV pink224_light (224,180,brightness);

CHSV aqua128 (128,180,brightness);
CHSV aqua128_light (128,180,brightness);

CHSV vert96 (96,180,brightness);
CHSV vert96_light (96,180,brightness);

CHSV violet200 (32,180,brightness);
CHSV jaune200_light (32,180,brightness);

CHSV violet238 (238,180,brightness);
CHSV violet238_light (238,180,brightness);

CHSV vert120 (120,180,50);
CHSV vert120_light (120,180,60);

CHSV white0 (0,0,40);

CHSV vert85 (85,180,brightness);
CHSV vert85_light (85,180,brightness);

CHSV azure148 (148,180,brightness);
CHSV azure148_light (148,150,brightness);

CHSV magenta201 (201,180,brightness);
CHSV magenta201_light (201,180,brightness);

CHSV chartreuse64 (64,180,brightness);
CHSV orange19 (16,180,brightness);

CHSV jaune40 (40,150,brightness);
CHSV black (0,0,0);

CHSV red (230,150,brightness);
     
CHSV CHSV_array[24][2]={      
      {purple196_light, orange19},
      {orange19, white0},
      {white0, vert102_light},
      {vert102_light, jaune200_light},
      {jaune200_light, red},
      {red, azure148_light},
      {azure148_light, orange19},
      {orange19, magenta201_light},
      {magenta201_light, black},
      {black, jaune40},
      {jaune40, violet238},
      {violet238, vert102_light},
      {vert102_light, jaune200_light},
      {jaune200_light, white0},
      {white0, vert102_light},
      {vert102_light, jaune200_light},
      {jaune200_light, red},
      {red, azure148_light},
      {azure148_light, orange19},
      {orange19, magenta201_light},
      {magenta201_light, black},
      {black, jaune40},
      {jaune64, violet238},
      {violet238, vert120},
};

    int minutetest = 0;
    int hourTest = 0;
// This function sets up the leds and tells the controller about them

void setup() {
    //sanity check delay - allows reprogramming if accidently blowing power w/leds
    delay(1000);
    FastLED.clear();

    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
   
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    Wire.begin();
    setDateTime();    
        

    //Wire.beginTransmission(DS1307_ADDRESS);
    //Wire.write(zero);
    //Wire.endTransmission();
    //Wire.requestFrom(DS1307_ADDRESS, 7);   
    //int minute = bcdToDec(Wire.read());
    //int division = minute / 3.75;
    //limit = division + 1;
    
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    //FastLED.setBrightness(BRIGHTNESS);
    Serial.begin(9600);  


}

char Time[] = "  :  :  ";
char Calendar[] = "  /  /20  ";
byte i, second, minute, hour, day, date, month, year, temperature_lsb;

// start test date
void setDateTime()
{
  byte second = 50;   //0-59
  byte minute = 31;   //0-59
  byte hour = 23;     //0-23
  byte weekDay = 6;  //1-7
  byte monthDay= 16; //1-31
  byte month = 10;     //1-12
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

void fillLine(int index, CRGB color, int posCouleur)
{ 
  for (int i = 0; i < 9; i++)
  {
    //leds[ledMatrix[index - 1][i]] = color;
     CHSV colorHour =  CHSV_array[posCouleur][0];
     colorHour.v = brightness;
    leds[ledMatrix[index - 1][i]] = colorHour;
  }
}
void fillLine2(int index, CRGB color, int posCouleur)
{  
  for (int i = 0; i < 9; i++)
  {
    //leds[ledMatrix[index - 1][i]] = color;
    CHSV colorHour =  CHSV_array[posCouleur][1];
    colorHour.v = brightness;
    leds[ledMatrix[index - 1][i]] = colorHour;
  }
}

void clearLine(int index)
{
  for (int i = 0; i < 9; i++)
  {
    //leds[ledMatrix[index - 1][i]] = CHSV(0, 0, 100);
    leds[ledMatrix[index - 1][i]] = CHSV(0, 0, 0);
  }
}

void loop() {


  // Buttons to modify Limit
  if (!digitalRead(button1))
  {
    if (limit != 16)
    {
      limit = limit + 1;
    }
    brightness = brightness + 20;
    if (brightness > 120)
    {
      brightness = 40;
    }
    Serial.println("a pressed");
     Serial.println(brightness);
  }
  
  if (!digitalRead(button2))
  {
    if (limit != 0)
    {
      limit = limit - 1;
    }
    Serial.println("b pressed");
  }


 
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

  minutetest = minutetest + 2;
  if(minutetest == 60){
    hourTest = (hourTest+1)%24;
    minutetest = 0;
    colorIndex = (colorIndex + 1)%9 ;
  };
  int division = minutetest / 3.75;
  int newLimit = division + 1 ;

  //int division = minute / 3.75;
  //int newLimit = division + 1;
  //Serial.print("time : ");
  //Serial.print(hour);
  //Serial.print(" : ");
  //Serial.print(minute);
  //Serial.print(" : ");
  //Serial.print(second);
    
  //Serial.println("Limit : ");
  //Serial.println(limit); 



  
 
  // Main loop for leds
   for (int z = 1; z < limit; z++)
  {
    fillLine2(z, colorHSV2, colorIndex);
  }
  delay(200);
  for (int y = limit + 1; y <= 16; y++)
  {
    fillLine(y, colorHSV1, colorIndex);
  }
  clearLine(limit);

  // test case 
  //fillLine(limit, colorRGB2);
  //fill_solid( leds, NUM_LEDS, CRGB(50,0,200));
  //fill_solid(leds, NUM_LEDS, colorHSV1);

  
  
  uint8_t colr = random()+millis()/100;
  
  // Led pour l'heure : all on  
  for (int hourLeds = 112; hourLeds <= 124; hourLeds++){
        leds[hourLeds] = CHSV(0, 0, 0);
  }
  CHSV colorHour =  CHSV_array[colorIndex][0];
  colorHour.v = 180;
  switch (hourTest%12) {
      case 0:
        leds[114] = colorHour;
        break;
      case 1:
        leds[113] = colorHour;
        break;
      case 2:
        leds[112] = colorHour;
        break;
      case 3:
        leds[123] = colorHour;
        break;
      case 4:
        leds[122] = colorHour;
        break;
      case 5:
        leds[121] = colorHour;
        break;
      case 6:
        leds[120] = colorHour;
        break;
      case 7:
        leds[119] = colorHour;
        break;
      case 8:
        leds[118] = colorHour;
        break;
      case 9:
        leds[117] = colorHour;
        break;
      case 10:
        leds[116] = colorHour;
        break;
      case 11:
        leds[115] = colorHour;
        break;
      default:        
        break;
    }

  //colorIndex = (hourTest)%24;
  delay(100);
  FastLED.show();
  limit = newLimit;
     
}
