#ifndef __MINI_PAD_H__
#define __MINI_PAD_H__

#include <Arduino.h>

// #include <GyverOLED.h>
#include "ssd1306-oled.h"

#include "pin-config.h"
#include "app.h"

#define CORD_C 2100
#define CORD_D 1870
#define CORD_E 1670
#define CORD_f 1580
#define CORD_G 1400
#define CORD_R 0

#define BTN_RIGHT_A 4
#define BTN_RIGHT_B 3
#define BTN_LEFT_A 2
#define BTN_LEFT_B 1

class MiniPad
{
public:
    // GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> *screen;

    // MiniPad() : screen(new GyverOLED<SSD1306_128x64, OLED_NO_BUFFER>()){};

    SSD1306_OLED *screen;

    MiniPad() : screen(new SSD1306_OLED()) {};

    void begin(App *app);
    void loop();

    int getRightPot() { return getPotValue_(POT_RIGHT); }
    int getLeftPot() { return getPotValue_(POT_LEFT); }

    bool pressedRightA() { return pressedButton_ == BTN_RIGHT_A ? true : false; }
    bool pressedRightB() { return pressedButton_ == BTN_RIGHT_B ? true : false; }
    bool pressedLeftA() { return pressedButton_ == BTN_LEFT_A ? true : false; }
    bool pressedLeftB() { return pressedButton_ == BTN_LEFT_B ? true : false; }

    void playTone(unsigned long frequency, unsigned long duration);

    int getButtonValue_();

private:
    int buttonReading_;
    int pressedButton_;
    int getPotValue_(uint8_t potPin);

    App *currentApp;
};

#endif // __MINI_PAD_H__