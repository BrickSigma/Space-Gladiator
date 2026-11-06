#include "utils/camera_shake.hpp"

#include <cmath>

CameraShake::CameraShake(float max_range, unsigned int duration) : range(max_range)
{
    if (duration == 0)
    {
        decay = 0;
    }
    else
    {
        decay = max_range / static_cast<float>(duration); // Calculate the decay value
    }
}

void CameraShake::update()
{
    range -= decay;
    if (range <= 0)
    {
        range = 0;
        offset = {0, 0};
        return;
    }

    float y = static_cast<float>(GetRandomValue(-static_cast<int>(floor(range)), static_cast<int>(floor(range))));
    float x = static_cast<float>(GetRandomValue(-static_cast<int>(floor(range)), static_cast<int>(floor(range))));

    offset = {x, y};
}