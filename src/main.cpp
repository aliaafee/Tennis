#include <Arduino.h>

#include "mini-pad.h"
MiniPad gamepad = MiniPad();

#include "tennis.h"
Tennis tennis = Tennis(&gamepad);

void setup()
{
    tennis.begin();

    gamepad.begin(&tennis);
}

void loop()
{
    gamepad.loop();
}
