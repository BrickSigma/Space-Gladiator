#include "utils/math.hpp"

#include <numbers>

float deg2rad(float deg)
{
    return deg * std::numbers::pi_v<float> / 180.0f;
}

float rad2deg(float rad)
{
    return rad * 180.0f / std::numbers::pi_v<float>;
}