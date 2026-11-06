#ifndef CAMERA_SHAKE_HPP
#define CAMERA_SHAKE_HPP

#include <raylib.h>

// Special camera class used to handle camera shake for the game scene
class CameraShake
{
public:
    Vector2 offset{0.0f, 0.0f}; // Offset applied to camera
    float decay = 0;            // How quickly the shake effect wares off
    float range = 0;            // The maximum offset of the camera shake effect

    /**
     * Create a camera shake effect.
     *
     * @param max_range maximum starting range of the shake offset
     * @param duration how long (in frames) should the effect last
     */
    CameraShake(float max_range, unsigned int duration);
    ~CameraShake() = default;

    // Used to update the camera shake offset each frame
    void update();
};

#endif // CAMERA_SHAKE_HPP