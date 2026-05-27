#pragma once
#include <SDL.h>

struct Circle
{
    int centerX;
    int centerY;
    int radius;
    std::vector<SDL_Point> points;
};
