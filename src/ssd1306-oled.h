/*!
 * @file ssd1306-oled.h
 *
 * Part of SSD1306 library for monochrome OLED displays
 *
 */

#ifndef __SSD1306_OLED_H__
#define __SSD1306_OLED_H__

#include "Arduino.h"
#include "Wire.h"

#include "char-map.h"

#define OLED_WIDTH 128
#define OLED_HEIGHT_32 0x02
#define OLED_HEIGHT_64 0x12
#define OLED_64 0x3F
#define OLED_32 0x1F

#define OLED_DISPLAY_OFF 0xAE
#define OLED_DISPLAY_ON 0xAF

#define OLED_COMMAND_MODE 0x00
#define OLED_ONE_COMMAND_MODE 0x80
#define OLED_DATA_MODE 0x40
#define OLED_ONE_DATA_MODE 0xC0

#define OLED_ADDRESSING_MODE 0x20
#define OLED_HORIZONTAL 0x00
#define OLED_VERTICAL 0x01

#define OLED_NORMAL_V 0xC8
#define OLED_FLIP_V 0xC0
#define OLED_NORMAL_H 0xA1
#define OLED_FLIP_H 0xA0

#define OLED_CONTRAST 0x81
#define OLED_SETCOMPINS 0xDA
#define OLED_SETVCOMDETECT 0xDB
#define OLED_CLOCKDIV 0xD5
#define OLED_SETMULTIPLEX 0xA8
#define OLED_COLUMNADDR 0x21
#define OLED_PAGEADDR 0x22
#define OLED_CHARGEPUMP 0x8D

#define OLED_NORMALDISPLAY 0xA6
#define OLED_INVERTDISPLAY 0xA7

#define SSD1306_SCL A5
#define SSD1306_SDA A4
#define SSD1306_SA 0x3C

class SSD1306_OLED
{
public:
    SSD1306_OLED(uint8_t address = 0x3C) : _address(address){};

    void begin();
    void init();

    void sendCommand(uint8_t command);

    void beginCommand();
    void endCommand();
    void beginData();
    void endData();

    void write(uint8_t data);

    void setWindow(int x0, int y0, int x1, int y1);

    void fillscreen(uint8_t fill_Data);

    void clear();

    void fillRect(int x0, int y0, int w, int h, byte fill);

    void dot(int x, int y, byte fill = 1);
    void fastLineH(int y, int x0, int x1, byte fill = 1);
    void fastLineV(int x, int y0, int y1, byte fill = 1);

    void setCursorXY(int x, int y);
    void setCursor(int col, int row);
    uint8_t getFont(uint8_t font, uint8_t row);
    void writeChar(uint8_t data);
    void clearChar();

    void print(String &line);
    void print(int number);
    void clearPrint(int length);

    void fillCell(int col, int row);
    void clearCell(int col, int row);

private:
    const uint8_t _address = 0x3C;
    const uint8_t _maxRow = 8 - 1;
    const uint8_t _maxY = OLED_HEIGHT_64 - 1;
    const uint8_t _maxX = OLED_WIDTH - 1;
    uint8_t _scaleX = 1, _scaleY = 8;
    uint8_t _mode = 2;
    bool _inRange(int x, int mi, int ma);
    void _printDigits(int number);
};

#endif // __SSD1306_OLED_H__