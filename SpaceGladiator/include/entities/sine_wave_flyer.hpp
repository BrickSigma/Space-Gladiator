#ifndef SINE_WAVE_FLYER_HPP
#define SINE_WAVE_FLYER_HPP

#include "entities/entity.hpp"
#include "utils/animation.hpp"

class SineWaveFlyer : public Entity
{
public:
    /**
     * Create a sine-wave flyer
     *
     * @param pos
     * @param speacial when `true`, the flyer can drop a powerup
     */
    SineWaveFlyer(Vector2 pos, bool special);
    ~SineWaveFlyer();

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;

private:
    bool special{false};

    float origin{0};          // Original y-coordinate of flyer when spawned to use as center of sine wave
    const int max_height{20}; // Maximum height the flyer can go from its origin
    int frame_counter{0};
    const float period{90.0f}; // Period of sine wave (in frames)
    const int update_x_time{2};
    const float x_vel{-3.0f}; // X-axis velocity of the flyer

    Animation *animation{nullptr};
};

#endif // SINE_WAVE_FLYER_HPP