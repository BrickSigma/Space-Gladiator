#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <raylib.h>

class Animation
{
public:
    /**
     * Create an animation texture.
     * 
     * @param frames the texture containing the frames, aligned horizontally
     * @param frame_width the width of a single frame
     * @param speed controls the frame rate (a value of 1 means update every frame, a value of 5 means update every 5th frame)
     * @param loop loop the animation indefinitely
     */
    Animation(Texture frames, int frame_width, int speed, bool loop = false);
    ~Animation() = default;

    // Reset the animation
    void reset();

    // Draw the animation and update it's timers
    void draw(Vector2 pos);

    Texture frames;
    int frame_width = 0;
    int current_frame = 0;
    int total_frames = 0;
    int update_time = 1;
    int frame_counter = 0;
    bool loop;
    bool completed = false;
};

#endif // ANIMATION_HPP