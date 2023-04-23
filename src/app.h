#ifndef __APP_H__
#define __APP_H__

#include <Arduino.h>

class MiniPad;

class App
{
public:
    MiniPad *gamepad;
    App(MiniPad *gamepad_) { gamepad = gamepad_; };
    
    virtual void begin() {};
    virtual void loop() {};
};

#endif // __APP_H__