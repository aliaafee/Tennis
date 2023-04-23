#ifndef __MINI_OLED_H__
#define __MINI_OLED_H__

#include <Arduino.h>
#include <Wire.h>

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

static const uint8_t _oled_init[] PROGMEM = {
    OLED_DISPLAY_OFF,
    OLED_CLOCKDIV,
    0x80, // value
    OLED_CHARGEPUMP,
    0x14, // value
    OLED_ADDRESSING_MODE,
    OLED_VERTICAL,
    OLED_NORMAL_H,
    OLED_NORMAL_V,
    OLED_CONTRAST,
    0x7F, // value
    OLED_SETVCOMDETECT,
    0x40, // value
    OLED_NORMALDISPLAY,
    OLED_DISPLAY_ON,
};

class miniOLED
{
public:
    miniOLED(uint8_t address = 0x3C) : _address(address) {}

    void init(int __attribute__((unused)) sda = 0, int __attribute__((unused)) scl = 0)
    {

        Wire.begin();

        beginCommand();
        for (uint8_t i = 0; i < 15; i++)
            sendByte(pgm_read_byte(&_oled_init[i]));
        endTransm();
        beginCommand();
        sendByte(OLED_SETCOMPINS);
        sendByte(OLED_HEIGHT_64);
        sendByte(OLED_SETMULTIPLEX);
        sendByte(OLED_64);
        endTransm();
    }

    void clear() { fill(0); }

    void fill(uint8_t data)
    {

        setWindow(0, 0, _maxX, _maxRow);
        beginData();
        for (int i = 0; i < (1024); i++)
            sendByte(data);
        endTransm();
    }

    void beginCommand()
    {
        sendByteRaw(OLED_COMMAND_MODE);
    }

    void startTransm()
    {
        Wire.beginTransmission(_address);
    }

    void endTransm()
    {

        Wire.endTransmission();
        _writes = 0;
    }

    void sendByteRaw(uint8_t data)
    {
        Wire.write(data);
    }

    void sendByte(uint8_t data)
    {
        sendByteRaw(data);
    }

    void setWindow(int x0, int y0, int x1, int y1)
    {
        beginCommand();
        sendByteRaw(OLED_COLUMNADDR);
        sendByteRaw(constrain(x0, 0, _maxX));
        sendByteRaw(constrain(x1, 0, _maxX));
        sendByteRaw(OLED_PAGEADDR);
        sendByteRaw(constrain(y0, 0, _maxRow));
        sendByteRaw(constrain(y1, 0, _maxRow));
        endTransm();
    }

    void beginData()
    {
        startTransm();
        sendByteRaw(OLED_DATA_MODE);
    }

    void dot(int x, int y, byte fill = 1)
    {
        if (x < 0 || x > _maxX || y < 0 || y > _maxY)
            return;
        _x = 0;
        _y = 0;

        setWindow(x, y >> 3, _maxX, _maxRow);
        beginData();
        sendByte(1 << (y & 0b111)); // задвигаем 1 на высоту y
        endTransm();
    }

    void fastLineV(int x, int y0, int y1, byte fill = 1)
    {
        _x = 0;
        _y = 0;
        if (y0 > y1)
            _swap(y0, y1);
        if (x < 0 || x > _maxX)
            return;
        if (y0 == y1)
        {
            dot(x, y0, fill);
            return;
        }
        y1++;
        
            if (_buf_flag)
            {
                for (int y = y0; y < y1; y++)
                    dot(x, y, fill);
                return;
            }
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
                    writeData((fill >> (8 - height)) << shift, y0, x);
            }
            else
            {
                if (_inRange(y0, 0, _maxRow))
                    writeData(fill << shift, y0, x); // начальный кусок
                y0++;
                for (byte i = 0; i < numBytes - 1; i++, y0++)
                    if (_inRange(y0, 0, _maxRow))
                        writeData(fill, y0, x); // столбик
                if (_inRange(y0, 0, _maxRow))
                    writeData(fill >> shift2, y0, x); // нижний кусок
            }
            endTransm();
        
    }

    void writeData(byte data, byte offsetY = 0, byte offsetX = 0, int mode = 0)
    {

        sendByte(data);
    }

private:
    const uint8_t _address = 0x3C;
    const uint8_t _maxRow = (8) - 1;
    const uint8_t _maxY = (64) - 1;
    const uint8_t _maxX = OLED_WIDTH - 1;
    uint8_t _writes = 0;
    int _x = 0, _y = 0;

    void _swap(int &x, int &y)
    {
        int z = x;
        x = y;
        y = z;
    }
    bool _inRange(int x, int mi, int ma)
    {
        return x >= mi && x <= ma;
    }

    bool _buf_flag = false;
};
#endif // __MINI_OLED_H__