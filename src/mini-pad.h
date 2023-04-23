#ifndef __MINI_PAD_H__
#define __MINI_PAD_H__

#include <Arduino.h>

#include <GyverOLED.h>

#include "pin-config.h"
#include "app.h"

#define BTN_RIGHT_A 4
#define BTN_RIGHT_B 3
#define BTN_LEFT_A 2
#define BTN_LEFT_B 1

class MiniPad
{
public:
    GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> *screen;

    MiniPad() : screen(new GyverOLED<SSD1306_128x64, OLED_NO_BUFFER>()){};

    void begin(App *app);
    void loop();

    int getRightPot() { return getPotValue_(POT_RIGHT); }
    int getLeftPot() { return getPotValue_(POT_LEFT); }

    bool pressedRightA() { return pressedButton_ == BTN_RIGHT_A ? true : false; }
    bool pressedRightB() { return pressedButton_ == BTN_RIGHT_B ? true : false; }
    bool pressedLeftA() { return pressedButton_ == BTN_LEFT_A ? true : false; }
    bool pressedLeftB() { return pressedButton_ == BTN_LEFT_B ? true : false; }

    int getButtonValue_();
private:
    int buttonReading_;
    int pressedButton_;
    int getPotValue_(uint8_t potPin);
    
    App *currentApp;
};

#endif // __MINI_PAD_H__