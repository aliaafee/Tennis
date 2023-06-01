/*!
 * @file ssd1306-oled.cpp
 *
 * SSD1306 library for monochrome OLED displays
 * This is a stripped down version based on AlexGyver's 
 * GyverOLED https://github.com/GyverLibs/GyverOLED.
 * 
 * This library is optimized to work with Attiny85
 * and has limited functionality but uses much less
 * memory. Desiged to only work with 128x64 display
 * using the I2C interface.
 *
 */

#include "ssd1306-oled.h"

void SSD1306_OLED::begin()
{
  init();
}

void SSD1306_OLED::init()
{
  Wire.begin();
  sendCommand(OLED_DISPLAY_OFF);
  sendCommand(OLED_CLOCKDIV);
  sendCommand(0x80);
  sendCommand(OLED_CHARGEPUMP);
  sendCommand(0x14);
  sendCommand(OLED_ADDRESSING_MODE);
  sendCommand(OLED_VERTICAL);
  sendCommand(OLED_NORMAL_H);
  sendCommand(OLED_NORMAL_V);
  sendCommand(OLED_CONTRAST);
  sendCommand(0x7F); // value
  sendCommand(OLED_SETVCOMDETECT);
  sendCommand(0x40); // value
  sendCommand(OLED_NORMALDISPLAY);
  sendCommand(OLED_DISPLAY_ON);
}

void SSD1306_OLED::sendCommand(uint8_t command)
{
  Wire.beginTransmission(_address);
  Wire.write(OLED_COMMAND_MODE);
  Wire.write(command);
  Wire.endTransmission();
}

void SSD1306_OLED::beginCommand()
{
  Wire.beginTransmission(_address);
  Wire.write(OLED_COMMAND_MODE);
}

void SSD1306_OLED::endCommand()
{
  Wire.endTransmission();
}

void SSD1306_OLED::beginData()
{
  Wire.beginTransmission(_address);
  Wire.write(OLED_DATA_MODE);
}

void SSD1306_OLED::endData()
{
  Wire.endTransmission();
}

void SSD1306_OLED::write(uint8_t data)
{
  Wire.write(data);
}

void SSD1306_OLED::setWindow(int x0, int y0, int x1, int y1)
{
  beginCommand();
  write(OLED_COLUMNADDR);
  write(constrain(x0, 0, _maxX));
  write(constrain(x1, 0, _maxX));
  write(OLED_PAGEADDR);
  write(constrain(y0, 0, _maxRow));
  write(constrain(y1, 0, _maxRow));
  endCommand();
}

void SSD1306_OLED::fillscreen(uint8_t fill)
{
  for (int x = 0; x <= _maxX; x += 12)
  {
    for (int y = 0; y <= _maxRow; y += 1)
    {
      setWindow(x, y, x + 12, y);
      beginData();
      for (int i = 0; i < 12; i++)
      {
        write(fill);
      }
      endData();
    }
  }
}

void SSD1306_OLED::clear()
{
  fillscreen(0);
}

void SSD1306_OLED::fillRect(int x0, int y0, int w, int h, byte fill)
{
  if (fill)
    fill = 255;
  setWindow(x0, y0 >> 3, x0 + w, (y0 + h) >> 3);
  beginData();
  for (int i = 0; i < w * h; i++)
  {
    write(fill);
  }
  endData();
}

void SSD1306_OLED::dot(int x, int y, byte fill)
{
  setWindow(x, y >> 3, 1, 1);
  beginData();
  write(fill << (y & 0b111));
  endData();
}

void SSD1306_OLED::fastLineH(int y, int x0, int x1, byte fill)
{
  byte data = fill << (y & 0b111);
  y >>= 3;
  setWindow(x0, y, x1, y);
  beginData();
  for (int x = x0; x < x1; x++)
    write(data);
  endData();
}

void SSD1306_OLED::fastLineV(int x, int y0, int y1, byte fill)
{
  if (fill)
    fill = 255;
  byte shift = y0 & 0b111;
  byte shift2 = 8 - (y1 & 0b111);
  if (shift2 == 8)
    shift2 = 0;
  int height = y1 - y0;
  y0 >>= 3;
  y1 = (y1 - 1) >> 3;
  byte numBytes = y1 - y0;
  setWindow(x, y0, x, y1);
  beginData();
  if (numBytes == 0)
  {
    if (_inRange(y0, 0, _maxRow))
      write((fill >> (8 - height)) << shift);
  }
  else
  {
    if (_inRange(y0, 0, _maxRow))
      write(fill << shift);
    y0++;
    for (byte i = 0; i < numBytes - 1; i++, y0++)
      if (_inRange(y0, 0, _maxRow))
        write(fill);
    if (_inRange(y0, 0, _maxRow))
      write(fill >> shift2);
  }
  endData();
}

void SSD1306_OLED::setCursorXY(int x, int y)
{
  setWindow(x, y >> 3, x + _maxX, (y >> 3));
}

void SSD1306_OLED::setCursor(int col, int row)
{
  setWindow((col * 6), row, (col * 6) + _maxX, row);
}

uint8_t SSD1306_OLED::getFont(uint8_t font, uint8_t row)
{
  if (row > 4)
    return 0;
  font = font - '0' + 16;
  if (font <= 95)
  {
    return pgm_read_byte(&(_charMap[font][row]));
  }
  else if (font >= 96 && font <= 111)
  {
    return pgm_read_byte(&(_charMap[font + 47][row]));
  }
  else if (font <= 159)
  {
    return pgm_read_byte(&(_charMap[font - 17][row]));
  }
  else
  {
    return pgm_read_byte(&(_charMap[font - 1][row]));
  }
}

void SSD1306_OLED::writeChar(uint8_t data)
{
  beginData();

  for (uint8_t col = 0; col < 6; col++)
  {
    uint8_t bits = getFont(data, col);
    write(bits);
  }
  endData();
}

void SSD1306_OLED::clearChar()
{
  beginData();

  for (uint8_t col = 0; col < 6; col++)
  {
    write(0);
  }
  endData();
}

void SSD1306_OLED::print(String &line)
{
  for (unsigned int i = 0; i < line.length(); i++)
  {
    writeChar(line.charAt(i));
  }
}

void SSD1306_OLED::print(int number)
{
  _printDigits(number);
}

void SSD1306_OLED::clearPrint(int length)
{
  for (int i = 0; i < length; i++)
  {
    clearChar();
  }
}

void SSD1306_OLED::fillCell(int col, int row)
{
  setCursor(col, row);

  beginData();
  write(0x00);
  for (uint8_t col = 0; col < 5; col++)
  {
    write(0xfc);
  }
  endData();
}

void SSD1306_OLED::clearCell(int col, int row)
{
  setCursor(col, row);

  beginData();
  for (uint8_t col = 0; col < 6; col++)
  {
    write(0x00);
  }
  endData();
}

bool SSD1306_OLED::_inRange(int x, int mi, int ma)
{
  return x >= mi && x <= ma;
}

void SSD1306_OLED::_printDigits(int number)
{
  if (number >= 10)
    _printDigits(number / 10);

  int digit = number % 10;
  char c = '0' + digit;

  writeChar(c);
}
