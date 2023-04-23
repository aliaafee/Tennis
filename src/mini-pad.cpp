#include "mini-pad.h"

// MiniPad::MiniPad()
// {
//     screen = new GyverOLED<SSD1306_128x64, OLED_NO_BUFFER>();
//     // screen = new miniOLED();
// }

// MiniPad::~MiniPad()
// {
//     ;
// }

void MiniPad::begin(App *app)
{
    pinMode(POT_RIGHT, INPUT);
    pinMode(POT_LEFT, INPUT);

    pinMode(BTN_INPUT, INPUT_PULLUP);

    screen->init();
    screen->clear();

    currentApp = app;
}

void MiniPad::loop()
{
    if (currentApp == NULL)
        return;
    
    pressedButton_ = getButtonValue_();

    (currentApp)->loop();

    // screen->setCursor(0, 6);
    // screen->print("b=");
    // screen->print(buttonReading_);
}

int MiniPad::getPotValue_(uint8_t potPin)
{
    return analogRead(potPin);
}

int MiniPad::getButtonValue_()
{
    /*
    Button setup as potential divider and the reset pin used as input

        5v - 
            |
            1K
            |---/ S1---
            1K         |
            |---/ S2---|
            1k         |-----RST Pin (A0)
            |---/ S3---|
            1K         |
            |---/ S4---
            3K3
            |
        GND-

        0 no button pressed
    */

    buttonReading_ = analogRead(BTN_INPUT);

    if (buttonReading_ < 550) return 4;
    if (buttonReading_ < 650) return 3;
    if (buttonReading_ < 790) return 2;
    if (buttonReading_ < 920) return 1;

    return 0;


    //return analogRead(BTN_INPUT);
}
