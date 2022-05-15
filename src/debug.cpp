#include "debug.h"

#include <chrono>
#include <thread>
#include <ctime>

#define FRAME_UPDATE_INTERVAL 100   // ms

static clock_t s_lastFrameTime = 0;
static clock_t s_deltaTime = 0;

int ImmGraphics::CalculateFPS()
{
    static int _frame = 0;
    static clock_t _lastTime = 0;
    static int _fps = 0;
    
    clock_t now = clock();
    ++_frame;

    s_deltaTime = now - s_lastFrameTime;
    s_lastFrameTime = now;

    clock_t interval = now - _lastTime;
    if (interval >= FRAME_UPDATE_INTERVAL)
    {
        _fps = (float)CLOCKS_PER_SEC / interval * _frame;
        _frame = 0;
        _lastTime = now;
    }

    return _fps;
}

float ImmGraphics::getDeltaTime()
{
    return (float)s_deltaTime / CLOCKS_PER_SEC;
}

void ImmGraphics::WaitForSeconds(float seconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds((long)(seconds * 1000)));
}
