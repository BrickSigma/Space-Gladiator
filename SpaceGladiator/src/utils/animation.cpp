#include "utils/animation.hpp"

#include <stdexcept>

Animation::Animation(Texture frames_, int frame_width_, int speed, bool loop_)
{
    this->frames = frames_;
    this->update_time = speed;
    this->loop = loop_;
    this->frame_width = frame_width_;
    this->total_frames = frames_.width / frame_width_;

    if (this->total_frames <= 0)
        throw std::runtime_error("Loaded animation frames has invalid frame width or texture!");
}

void Animation::reset()
{
    completed = false;
    current_frame = 0;
}

void Animation::draw(Vector2 pos)
{
    if (!completed)
        DrawTextureRec(frames, {static_cast<float>(current_frame * frame_width), 0.0f, static_cast<float>(frame_width), static_cast<float>(frames.height)}, pos, WHITE);

    frame_counter++;
    if (frame_counter % update_time == 0)
    {
        current_frame++;
        if (current_frame >= total_frames) {
            if (loop) {
                current_frame = 0;
            } else {
                completed = true;
            }
        }
    }
}