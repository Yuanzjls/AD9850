#ifndef AnalogMeter_h_
#define AnalogMeter_h_
#include <MCUFRIEND_kbv.h>
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
class AnalogMeter
{
  public: 
  AnalogMeter(Adafruit_GFX *gfx, uint16_t x0, uint16_t y0, uint16_t w, uint16_t h,
  uint16_t start, uint16_t e, char *uint, char *string)
  {
     _gfx = gfx;
     _x0 = x0;
     _y0 = y0;
     _width = w;
     _height = h;
     _start = start;
     _end = e;
     _len =120;
     strcpy(_unit, uint);
     strcpy(_string, string);
   }

  Draw()
  {
      char buf[8];
      double interval = (_end - _start)/4;
      _gfx->fillRect(_x0, _y0, _width, _height, GREY);
      _gfx->fillRect(_x0 + 5, _y0+3, _width - 9, _height-7, WHITE);  
      _gfx->setTextColor(BLACK);  // Text colour
      
      int x_center = _width / 2 + _x0;
      int y_center = _height + 14 + _y0;
       // Draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
    for (int i = -50 ; i < 51; i += 5) {
      // Long scale tick length
       int tl = 15;
      
      // Coodinates of tick to draw
      float sx = cos((i - 90) * 0.0174532925);
      float sy = sin((i - 90) * 0.0174532925);
      uint16_t x0 = sx * (_len + tl) + x_center;
      uint16_t y0 = sy * (_len + tl) + y_center;
      uint16_t x1 = sx * _len + x_center;
      uint16_t y1 = sy * _len + y_center;
      
      // Coordinates of next tick for zone fill
      float sx2 = cos((i + 5 - 90) * 0.0174532925);
      float sy2 = sin((i + 5 - 90) * 0.0174532925);
      int x2 = sx2 * (_len + tl) + x_center;
      int y2 = sy2 * (_len + tl) + y_center;
      int x3 = sx2 * _len + x_center;
      int y3 = sy2 * _len + y_center;
      
   //    Yellow zone limits
        if (i >= -50 && i < 0) {
          _gfx->fillTriangle(x0, y0, x1, y1, x2, y2, YELLOW);
          _gfx->fillTriangle(x1, y1, x2, y2, x3, y3, YELLOW);
        }
        
        // Green zone limits
        if (i >= 0 && i < 25) {
          _gfx->fillTriangle(x0, y0, x1, y1, x2, y2, GREEN);
          _gfx->fillTriangle(x1, y1, x2, y2, x3, y3, GREEN);
        }
    
        // Orange zone limits
        if (i >= 25 && i < 50) {
          _gfx->fillTriangle(x0, y0, x1, y1, x2, y2, ORANGE);
          _gfx->fillTriangle(x1, y1, x2, y2, x3, y3, ORANGE);
        }
        
        // Short scale tick length
        if (i % 25 != 0) tl = 8;
        
        // Recalculate coords incase tick lenght changed
        x0 = sx * (_len + tl) + x_center;
        y0 = sy * (_len + tl) + y_center;
        x1 = sx * _len + x_center;
        y1 = sy * _len + y_center;
        
        // Draw tick
        _gfx->drawLine(x0, y0, x1, y1, BLACK);
        
        // Check if labels should be drawn, with position tweaks
        if (i % 25 == 0) {
          
          int w=4,de = 2;
          // Calculate label positions
          x0 = sx * (_len + tl + 10) + x_center;
          y0 = sy * (_len + tl + 10) + y_center;
          switch (i / 25) {
            case -2: 
              dtostrf(_start, 1, 0, buf);
              _gfx->setCursor(x0, y0-12);
              _gfx->print(buf);
            break;
            case -1:
              dtostrf((_start + 1 * interval)/(double)100, w, de, buf);
              _gfx->setCursor(x0-6, y0-9);
              _gfx->print(buf);
            break;
            case 0: 
    //          tft.drawCentreString("50", x0, y0 - 6, 2);
               dtostrf((_start + 2 * interval)/(double)100, w, de, buf);
              _gfx->setCursor(x0-5, y0-6);
              _gfx->print(buf);
             break;
            case 1: 
     //         tft.drawCentreString("75", x0, y0 - 9, 2);
               dtostrf((_start + 3 * interval)/(double)100, w, de, buf);
              _gfx->setCursor(x0-7, y0-9);
              _gfx->print(buf);
            break;
            case 2: 
    //          tft.drawCentreString("100", x0, y0 - 12, 2);
              dtostrf((_start + 4 * interval)/(double)100, w, de, buf);
              _gfx->setCursor(x0-12, y0-12);
              _gfx->print(buf);
             break;
          }
        }
        
        // Now draw the arc of the scale
        sx = cos((i + 5 - 90) * 0.0174532925);
        sy = sin((i + 5 - 90) * 0.0174532925);
        x0 = sx * _len + x_center;
        y0 = sy * _len + y_center;
        // Draw scale arc, don't draw the last part
        if (i < 50) _gfx->drawLine(x0, y0, x1, y1, BLACK);
    }
    _gfx->setCursor(_x0 + _width - 9 - 40,_y0+_height-7-20);
    _gfx->print(_string); // Units at bottom right
    _gfx->setCursor(_x0 + _width/2 - 7, _y0+_height - 70);
    _gfx->setTextSize(3);
    _gfx->print(_unit); // Units at bottom right
    _gfx->drawRect(_x0 + 5, _y0+3, _width - 9, _height-7, BLACK); // Draw bezel line
  }
void plotNeedle(int value, byte ms_delay)
{
  static int old_analog =  -999; 
  static float ltx = 0;  
  static uint16_t osx = 120, osy = 120; // Saved x & y coords
  static uint32_t updateTime = 0;       // time for next update

  int x_center = _width / 2 + _x0;
  int y_center = _height + 14 + _y0;
  _gfx->setTextColor(BLACK, WHITE);
  char buf[8]; 
  dtostrf((double)value /100, 4, 2, buf);

  _gfx->setTextSize(1);
  _gfx->setCursor(_x0+20, _y0+_height-7-20);
  _gfx->println(buf); //Erase old value

  
//  if (value < -10) value = -10; // Limit value to emulate needle end stops
//  if (value > 110) value = 110;

  // Move the needle util new value reached
  while (!(value == old_analog)) {
    if (old_analog < value) old_analog++;
    else old_analog--;
    
    if (ms_delay == 0) old_analog = value; // Update immediately id delay is 0
    
    float sdeg = map(old_analog, _start, _end, -139, -39); // Map value to angle 
    // Calcualte tip of needle coords
    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    // Calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg+90) * 0.0174532925);
    
    // Erase old needle image
    _gfx->drawLine(x_center + 20 * ltx - 1, y_center - 20, osx - 1, osy, WHITE);
    _gfx->drawLine(x_center + 20 * ltx, y_center - 20, osx, osy, WHITE);
    _gfx->drawLine(x_center + 20 * ltx + 1, y_center - 20, osx + 1, osy, WHITE);
    
    // Re-plot text under needle
    _gfx->setTextColor(BLACK);
    _gfx->setCursor( _x0 + _width/2 - 7, _y0+_height - 70);
    _gfx->setTextSize(3);
    _gfx->print(_unit); // Units at bottom right
    //tft.drawCentreString("%RH", 120, 70, 4); // // Comment out to avoid font 4
    
    // Store new needle end coords for next erase
    ltx = tx;
    osx = sx * 118 + x_center;
    osy = sy * 118 + y_center;
    
    // Draw the needle in the new postion, magenta makes needle a bit bolder
    // draws 3 lines to thicken needle
    _gfx->drawLine(x_center + 20 * ltx - 1, y_center - 20, osx - 1, osy, RED);
    _gfx->drawLine(x_center + 20 * ltx, y_center - 20, osx, osy, MAGENTA);
    _gfx->drawLine(x_center + 20 * ltx + 1, y_center - 20, osx + 1, osy, RED);
    
    // Slow needle down slightly as it approaches new postion
    if (abs(old_analog - value) < 10) ms_delay += ms_delay / 5;
    
    // Wait before next update
    delayMicroseconds(ms_delay);
  }
}
void digitalMeter(int x,int y)
{
  xpos = x;
  ypos = y;// was 134
  _gfx->fillRect(xpos - 52, ypos - 5, 2 * 77, 59, GREY);
  _gfx->fillRect(xpos - 49, ypos - 2, 2 * 74, 53, BLACK);
  _gfx->setTextColor(7 << 11, BLACK); // Plot over numbers in dim red

  _gfx->setTextSize(6);
  _gfx->setCursor(xpos - 47, ypos+2);
  _gfx->println("8888"); //Erase old value
//  tft.drawNumber(888, xpos - 77, ypos, 7);
  
}

// #########################################################################
// Update digital meter reading
// #########################################################################
void showDigital(int value)
{
  static int old_digital = -999;
  if (value==old_digital) return; // return if no change to prevent flicker
//  if (value < 0) value = 0; //Constrain lower limit to 0
//  if (value > 999) value = 999; //Constrain upper limit to 999
  
//  int xpos = 140, ypos = 134; // Position with location tweak
  _gfx->setTextColor(7 << 11, BLACK); // Plot over numbers in dim red
  _gfx->setTextSize(6);
  _gfx->setCursor(xpos - 47, ypos+2);
  _gfx->println("8888"); //Erase old value
  
  // Nb. 32 pixels wide +2 gap per digit
  char buf[8]; 
  dtostrf((double)value, 4, 0, buf);
  // Update with new value
  _gfx->setTextColor(RED); // Dont draw background to leave dim segments
//  if (value < 10) tft.drawNumber(value, xpos+19, ypos, 7);
//  else if (value < 100) tft.drawNumber(value, xpos - 14, ypos, 7);
//  else tft.drawNumber(value, xpos - 47, ypos, 7);
  _gfx->setTextSize(6);
  _gfx->setCursor(xpos - 47, ypos+2);
  _gfx->println(buf); //Erase old value
  old_digital = value;
}
  private:
  Adafruit_GFX *_gfx;
  uint16_t _x0, _y0, _width, _height, xpos, ypos;
  uint16_t _start, _end, _len;
  char  _unit[10], _string[10]; 
};
#endif

