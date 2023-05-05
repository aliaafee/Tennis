#include "tennis.h"

void Tennis::begin()
{
    rightScore = 0;
    leftScore = 0;

    rightPaddle.x = 128 - 2;
    rightPaddle.y = 32;
    rightPaddle.h = 10;

    leftPaddle.x = 1;
    leftPaddle.y = 32;
    leftPaddle.h = 10;

    currentLoop = &Tennis::leftServeLoop;
    updateScore();
}

void Tennis::loop()
{
    while ((millis() - lastFrame) < FRAME_TIME)
        ;
    lastFrame = millis();
    (this->*currentLoop)();
}

void Tennis::playLoop()
{
    ball.x += ball.v_x;
    ball.y += ball.v_y;

    paddleInput();

    if (ball.y >= (64 - 1) || ball.y < 0)
    {
        gamepad->playTone(CORD_G, 70);
        ball.v_y = -ball.v_y;
    }

    if (ball.x < 4)
    {
        if (hitPaddle(ball, leftPaddle))
        {
            ball.v_x = -ball.v_x;
            gamepad->playTone(CORD_C, 100);
        }
        else
        {
            rightScore += 1;
            updateScore();
            playDeadTone();
            currentLoop = &Tennis::rightServeLoop;
        }
    }

    if (ball.x >= (124))
    {
        if (hitPaddle(ball, rightPaddle))
        {
            ball.v_x = -ball.v_x;
            gamepad->playTone(CORD_C, 100);
        }
        else
        {
            leftScore += 1;
            updateScore();
            playDeadTone();
            currentLoop = &Tennis::leftServeLoop;
        }
    }

    updateBall();

    updatePaddle(rightPaddle);

    updatePaddle(leftPaddle);

    // if (gamepad->pressedRightA())
    // {
    //     gamepad->screen->setCursorXY(50, 0);
    //     gamepad->screen->print("R-A");
    //     return;
    // }
    // if (gamepad->pressedRightB())
    // {
    //     gamepad->screen->setCursorXY(50, 0);
    //     gamepad->screen->print("R-B");
    //     return;
    // }

    // if (gamepad->pressedLeftA())
    // {
    //     gamepad->screen->setCursorXY(50, 0);
    //     gamepad->screen->print("L-A");
    //     return;
    // }

    // if (gamepad->pressedLeftB())
    // {
    //     gamepad->screen->setCursorXY(50, 0);
    //     gamepad->screen->print("L-B");
    //     return;
    // }

    // gamepad->screen->setCursorXY(50, 0);
    // gamepad->screen->print("---");
}

void Tennis::deadLoop()
{
}

void Tennis::rightServeLoop()
{
    paddleInput();

    ball.x = rightPaddle.x - 5;
    ball.y = rightPaddle.y;

    updateBall();

    updatePaddle(leftPaddle);
    updatePaddle(rightPaddle);

    if (gamepad->pressedRightA())
    {
        ball.v_x = -2;
        ball.v_y = 2;
        currentLoop = &Tennis::playLoop;
        gamepad->playTone(CORD_C, 100);
        return;
    }

    if (gamepad->pressedRightB())
    {
        ball.v_x = -2;
        ball.v_y = -2;
        currentLoop = &Tennis::playLoop;
        gamepad->playTone(CORD_C, 100);
        return;
    }
}

void Tennis::leftServeLoop()
{
    paddleInput();

    ball.x = leftPaddle.x + 5;
    ball.y = leftPaddle.y;

    updateBall();

    updatePaddle(leftPaddle);
    updatePaddle(rightPaddle);

    if (gamepad->pressedLeftA())
    {
        ball.v_x = 2;
        ball.v_y = 2;
        currentLoop = &Tennis::playLoop;
        gamepad->playTone(CORD_C, 100);
        return;
    }

    if (gamepad->pressedLeftB())
    {
        ball.v_x = 2;
        ball.v_y = -2;
        currentLoop = &Tennis::playLoop;
        gamepad->playTone(CORD_C, 100);
        return;
    }
}

void Tennis::paddleInput()
{
    rightPaddle.y = map(gamepad->getRightPot(), 0, 1023, 5, 59);
    leftPaddle.y = map(gamepad->getLeftPot(), 0, 1023, 59, 5);
}

void Tennis::drawBall(int &x, int &y, byte color)
{
    gamepad->screen->fastLineV(x - 1, y - 1, y + 1, color);
    gamepad->screen->fastLineV(x, y - 1, y + 1, color);
    gamepad->screen->fastLineV(x + 1, y - 1, y + 1, color);
}

void Tennis::drawPaddle(int &x, int y, int h, byte color)
{
    gamepad->screen->fastLineV(x - 1, y - h / 2, y + h / 2, color);
    gamepad->screen->fastLineV(x, y - h / 2, y + h / 2, color);
    gamepad->screen->fastLineV(x + 1, y - h / 2, y + h / 2, color);
}

bool Tennis::hitPaddle(Ball &b, Paddle &paddle)
{
    int h2 = (paddle.h + 4) / 2;

    if (ball.y < paddle.y - h2)
        return false;

    if (ball.y > paddle.y + h2)
        return false;

    return true;
}

void Tennis::updateScore()
{
    gamepad->screen->setCursorXY(34, 0);
    gamepad->screen->print(leftScore);

    gamepad->screen->setCursorXY(93, 0);
    gamepad->screen->print(rightScore);
}

void Tennis::updateBall()
{
    if (ball.x != ball.p_x || ball.y != ball.p_y)
    {
        drawBall(ball.p_x, ball.p_y, 0);
        drawBall(ball.x, ball.y, 1);
        ball.p_x = ball.x;
        ball.p_y = ball.y;
    }
}

void Tennis::updatePaddle(Paddle &paddle)
{
    if (paddle.y != paddle.p_y)
    {
        drawPaddle(paddle.x, paddle.p_y, paddle.h, 0);
        drawPaddle(paddle.x, paddle.y, paddle.h, 1);
        paddle.p_y = paddle.y;
    }
}

void Tennis::playDeadTone()
{
    gamepad->playTone(CORD_C, 50);
    delay(50);
    gamepad->playTone(CORD_D, 50);
    delay(50);
    gamepad->playTone(CORD_E, 50);
}
