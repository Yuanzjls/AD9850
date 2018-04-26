/*
 An example analogue meter using a ILI9341 TFT LCD screen

 This example uses the hardware SPI only and a board based
 on the ATmega328

 Needs Fonts 2, and 7 (also Font 4 if using large scale label)

 Comment out lines 153 and 197 to reduce needle flicker and
 to remove need for Font 4 (which uses ~8k of FLASH!)
 
 Alan Senior 23/2/2015
 */

// These are the connections for the UNO to display
#define sclk 13  // Don't change
#define mosi 11  // Don't change
#define cs   10  // If cs and dc pin allocations are changed then 
#define dc   9   // comment out #define F_AS_T line in "Adafruit_ILI9341_FAST.h"
                 // (which is inside "Adafriit_ILI9341_AS" library)

#define rst  7  // Can alternatively connect this to the Arduino reset

//#include <Adafruit_GFX_AS.h>     // Core graphics library
#include <MCUFRIEND_kbv.h> // Fast hardware-specific library
#include "AnalogMeter.h"
#include "adafruit_GFX_Ext.h"
MCUFRIEND_kbv tft;

Adafruit_GFX_Button Button;
#define LTBLUE    0xB6DF
#define LTTEAL    0xBF5F
#define LTGREEN         0xBFF7
#define LTCYAN    0xC7FF
#define LTRED           0xFD34
#define LTMAGENTA       0xFD5F
#define LTYELLOW        0xFFF8
#define LTORANGE        0xFE73
#define LTPINK          0xFDDF
#define LTPURPLE  0xCCFF
#define LTGREY          0xE71C

#define BLUE            0x001F
#define TEAL    0x0438
#define GREEN           0x07E0
#define CYAN          0x07FF
#define RED           0xF800
#define MAGENTA       0xF81F
#define YELLOW        0xFFE0
#define ORANGE        0xFD20
#define PINK          0xF81F
#define PURPLE    0x801F
#define GREY        0xC618
#define WHITE         0xFFFF
#define BLACK         0x0000

#define DKBLUE        0x000D
#define DKTEAL    0x020C
#define DKGREEN       0x03E0
#define DKCYAN        0x03EF
#define DKRED         0x6000
#define DKMAGENTA       0x8008
#define DKYELLOW        0x8400
#define DKORANGE        0x8200
#define DKPINK          0x9009
#define DKPURPLE      0x4010
#define DKGREY        0x4A49

  // Setup data buffer.
String inString;
int inByte = 0; // Incoming serial data byte.
int outputValue; // Sensor data to send to Processing.


float ltx = 0;    // Saved x coord of bottom of needle
uint16_t osx = 120, osy = 120; // Saved x & y coords
uint32_t updateTime = 0;       // time for next update
bool using_seril = true;
int old_analog =  -999; // Value last displayed
int old_digital = -999; // Value last displayed
int delay_ms = 0;
Adafruit_GFX_Buttonx Button1, Button2, Button3, Button4, Textmenu1, Textmenu2, Toptext;
char fre[10];
void setup(void) {
  uint16_t ID = tft.readID(); //
  if (ID == 0xD3D3)
  { 
    ID = 0x9481; // write-only shield
  }
  tft.begin(ID);
  tft.setRotation(1);
  Serial.begin(9600);
  tft.fillScreen(BLACK);

  Button1.initButtonUL(&tft, 10, 70, 50, 20, DKGREY, DKGREY, BLACK, YELLOW, "LSB", 2);
  Button2.initButtonUL(&tft, 80, 70, 50, 20, DKGREY, DKGREY, BLACK, YELLOW, "USB", 2);
  Button3.initButtonUL(&tft, 10, 95, 50, 20, DKGREY, DKGREY, BLACK, YELLOW, "C W", 2);
  Button4.initButtonUL(&tft, 80, 95, 50, 20, DKGREY, DKGREY, BLACK, YELLOW, "A M", 2);
  Textmenu1.initButtonUL(&tft, 210, 70, 100, 20, WHITE, BLACK, WHITE, WHITE, "100Hz", 2);
  Textmenu2.initButtonUL(&tft, 210, 95, 100, 20, WHITE, BLACK, WHITE, WHITE, "", 2);
  Toptext.initButtonUL(&tft, 5, 5, 310, 40, WHITE, BLACK, GREEN, GREEN, "7,100,000", 4);
  
  tft.setCursor(160, 70 + (20/2) - (4 * 2));
  tft.setTextSize(2);
  tft.print("STEP");
  tft.setCursor(160, 95 + (20/2) - (4 * 2));
  tft.setTextSize(2);
  tft.print("RIT");
  
  Textmenu1.drawButton();
  Textmenu2.drawButton();
  Toptext.drawButton();
  
  tft.setCursor(5, 130);
  tft.setTextSize(2);
  tft.setTextColor(DKGREY, BLACK);
  tft.print("P:");
  tft.setCursor(25, 137);
  tft.setTextSize(1);
  tft.print("1     3      5           10          ");
  tft.setCursor(30, 135);
  tft.print(".....");
  tft.setCursor(70, 135);
  tft.print(".....");
  tft.setCursor(110, 135);
  tft.print("...........");
  tft.setCursor(190, 135);
  tft.print("...........");


  tft.setCursor(5, 150);
  tft.setTextSize(2);
  tft.setTextColor(DKGREY, BLACK);
  tft.print("S:");
  tft.setCursor(25, 157);
  tft.setTextSize(1);
  tft.print("1     3        6           9   Over          ");
  tft.setCursor(30, 155);
  tft.print(".....");
  tft.setCursor(70, 155);
  tft.print(".......");
  tft.setCursor(120, 155);
  tft.print("...........");
  tft.setCursor(193, 155);
  tft.print("...");
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  // Wait for serial port to connect. Needed for native USB port only.
  while (!Serial)
  {
    ; // Do nothing until connection established.
  }

  // Establish contact by sending initial data to Processing.
  // Loop on this until Processing responds by sending a request for sensor data.
  // Then start the loop().
}

bool invert = false;
void loop() 
{
  invert = !invert;
  Button1.drawButton(invert);
  Button2.drawButton(!invert);
  Button3.drawButton(invert);
  Button4.drawButton(!invert);
  delay(1000);
}
