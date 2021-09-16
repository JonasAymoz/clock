#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>

// Led config
#define NUM_LEDS 140
#define BRIGHTNESS 80
#define DATA_PIN 2

// Screen Config
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4
#define DS1307_ADDRESS 0x68
Adafruit_SSD1306 display(OLED_RESET);
byte zero = 0x00; // hack fix

// Else config
#define button1 9 // Button B1 is connected to Arduino pin 9
#define button2 8 // Button B2 is connected to Arduino pin 8
#define buzzer 3

// Config setup
int ledMatrix[16][9] = {
    {1000, 1000, 1000, 47, 48, 79, 1000, 1000, 1000},
    {1000, 1000, 18, 46, 49, 78, 80, 1000, 1000},
    {1000, 17, 19, 45, 50, 77, 81, 105, 1000},
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

CRGB leds[NUM_LEDS];
int limit = 10;

// ------- COLORZ ---------
// color1 : blue green
//CRGB colorRGB = CRGB(0,128,15);
//CRGB colorRGB2 = CRGB(20,49,100);

// color2 : violet vert
CRGB colorRGB = CRGB(238, 100, 238);
CRGB colorRGB2 = CRGB(0, 180, 20);

// color2 : jaune violet
//CRGB colorRGB = CRGB(238,100,238);
//CRGB colorRGB2 = CRGB(250,250,20);

void setup(void)
{

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr
  setDateTime();
  Wire.begin();
  // init done

  // Clear the display buffer.
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE, BLACK);

  //Init Led stripe
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}

char Time[] = "  :  :  ";
char Calendar[] = "  /  /20  ";
char temperature[] = " 00.00";
char temperature_msb;
byte i, second, minute, hour, day, date, month, year, temperature_lsb;

void display_day()
{
  switch (day)
  {
  case 1:
    draw_text(0, 0, " SUNDAY  ", 1);
    break;
  case 2:
    draw_text(0, 0, " MONDAY  ", 1);
    break;
  case 3:
    draw_text(0, 0, " TUESDAY ", 1);
    break;
  case 4:
    draw_text(0, 0, "WEDNESDAY", 1);
    break;
  case 5:
    draw_text(0, 0, "THURSDAY ", 1);
    break;
  case 6:
    draw_text(0, 0, " FRIDAY  ", 1);
    break;
  default:
    draw_text(0, 0, "SATURDAY ", 1);
  }
}

void DS3231_display()
{
  // Convert BCD to decimal
  second = (second >> 4) * 10 + (second & 0x0F);
  minute = (minute >> 4) * 10 + (minute & 0x0F);
  hour = (hour >> 4) * 10 + (hour & 0x0F);
  date = (date >> 4) * 10 + (date & 0x0F);
  month = (month >> 4) * 10 + (month & 0x0F);
  year = (year >> 4) * 10 + (year & 0x0F);
  // End conversion

  Time[7] = second % 10 + 48;
  Time[6] = second / 10 + 48;
  Time[4] = minute % 10 + 48;
  Time[3] = minute / 10 + 48;
  Time[1] = hour % 10 + 48;
  Time[0] = hour / 10 + 48;
  Calendar[9] = year % 10 + 48;
  Calendar[8] = year / 10 + 48;
  Calendar[4] = month % 10 + 48;
  Calendar[3] = month / 10 + 48;
  Calendar[1] = date % 10 + 48;
  Calendar[0] = date / 10 + 48;

  draw_text(60, 0, "<3 jonas", 1); // Display the date (format: dd/mm/yyyy)
  draw_text(10, 18, "<3 MOBY", 2); // Display the time
  //draw_text(75, 55, temperature, 1);                  // Display the temperature
}

void blink_parameter()
{
  byte j = 0;
  while (j < 10 && digitalRead(button1) && digitalRead(button2))
  {
    j++;
    delay(25);
  }
}

// start test date
void setDateTime()
{

  byte second = 00;   //0-59
  byte minute = 18;   //0-59
  byte hour = 21;     //0-23
  byte weekDay = 4;   //1-7
  byte monthDay = 14; //1-31
  byte month = 5;     //1-12
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

void printDate()
{

  // Reset the register pointer
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

  //print the date EG 3/1/11 23:59:59

  /*String time = String(hour) + "-" + String(minute) + "-" + String(second);
Serial.print(hour);
Serial.print(":");
Serial.print(minute);
Serial.print(":");
Serial.print(second);
Serial.println(time);*/

  Time[7] = second % 10 + 48;
  Time[6] = second / 10 + 48;
  Time[4] = minute % 10 + 48;
  Time[3] = minute / 10 + 48;
  Time[1] = hour % 10 + 48;
  Time[0] = hour / 10 + 48;
  draw_text(0, 0, Time, 1);
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

void getDate()
{

  // Reset the register pointer
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

  Wire.write(zero); //start

  Wire.endTransmission();
}

///////////
// UTILS
///////////

void draw_text(byte x_pos, byte y_pos, char *text, byte text_size)
{
  display.setCursor(x_pos, y_pos);
  display.setTextSize(text_size);
  display.print(text);
  display.display();
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

//////////////////////
// MAIN LOOP
//////////////////////

void loop()
{

  // Buttons to modify Limit
  if (!digitalRead(button1))
  {
    if (limit != 16)
    {
      limit = limit + 1;
    }
    Serial.println("a pressed");
  }

  if (!digitalRead(button2))
  {
    if (limit != 0)
    {
      limit = limit - 1;
    }
    Serial.println("b pressed");
  }

  // horloge part
  printDate();
  getDate();

  // delay(3000);

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

  int limitMinutes = minute;
  int division = minute / 3.75;
  int newLimit = division + 1;
  Serial.print(newLimit);
  //Serial.print("secondes : ");
  //Serial.print(second);
  //Serial.print(" - Limite  : ");
  //Serial.print(limitSecond);
  //Serial.println("-");

  // Main loop for leds
  for (int y = limit + 1; y <= 16; y++)
  {
    fillLine(y, colorRGB2);
  }
  clearLine(limit);

  for (int z = 0; z < limit; z++)
  {
    fillLine(z, colorRGB);
  }
  // hour display
  //leds[112+(hour%12)] = CHSV(0,0,0);
  //leds[1] = colorRGB2;
  //leds[101] = CRGB::Blue;
  //leds[102] = CRGB::Yellow;
  //leds[90] = CRGB::Pink;
  //fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  
  DS3231_display();
  delay(100);
  Wire.endTransmission();
  limit = newLimit;

  /*if (limit == 16){
    color1 = (color1+40)%300;
    color2 = (color2+100)%300;
    limit = 0;
  }*/
}

// USELESS FOR NOW
byte edit(byte x_pos, byte y_pos, byte parameter)
{
  char text[3];
  sprintf(text, "%02u", parameter);
  while (!digitalRead(button1))
    ; // Wait until button B1 released
  while (true)
  {
    while (!digitalRead(button2))
    { // If button B2 is pressed
      parameter++;
      if (i == 0 && parameter > 31) // If date > 31 ==> date = 1
        parameter = 1;
      if (i == 1 && parameter > 12) // If month > 12 ==> month = 1
        parameter = 1;
      if (i == 2 && parameter > 99) // If year > 99 ==> year = 0
        parameter = 0;
      if (i == 3 && parameter > 23) // If hours > 23 ==> hours = 0
        parameter = 0;
      if (i == 4 && parameter > 59) // If minutes > 59 ==> minutes = 0
        parameter = 0;
      sprintf(text, "%02u", parameter);
      draw_text(x_pos, y_pos, text, 1);
      delay(200); // Wait 200ms
    }
    draw_text(x_pos, y_pos, "  ", 1);
    blink_parameter();
    draw_text(x_pos, y_pos, text, 1);
    blink_parameter();
    if (!digitalRead(button1))
    {                   // If button B1 is pressed
      i++;              // Increament 'i' for the next parameter
      return parameter; // Return parameter value and exit
    }
  }
}
