#ifndef __TENNIS_H__
#define __TENNIS_H__

#include "app.h"
#include "mini-pad.h"

#define FRAME_TIME 30

struct Ball
{
    int p_x;
    int p_y;
    int x;
    int y;
    int v_x;
    int v_y;
};

struct Paddle
{
    int p_y;
    int x;
    int y;
    int h;
};

class Tennis : public App
{
public:
    Tennis(MiniPad *gamepad_) : App(gamepad_){

                                };

    void begin();
    void loop();

private:
    unsigned long lastFrame;

    Ball ball;
    Paddle rightPaddle;
    Paddle leftPaddle;

    int rightScore;
    int leftScore;

    void (Tennis::*currentLoop)();
    void playLoop();
    void deadLoop();
    void rightServeLoop();
    void leftServeLoop();

    void paddleInput();

    void drawBall(int &x, int &y, byte color);
    void drawPaddle(int &x, int y, int h, byte color);

    bool hitPaddle(Ball &b, Paddle &paddle);
    void updateScore();
    void updateBall();
    void updatePaddle(Paddle &paddle);
};

#endif // __TENNIS_H__