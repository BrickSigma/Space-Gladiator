#ifndef SINE_WAVE_FLYER_HPP
#define SINE_WAVE_FLYER_HPP

#include "entities/entity.hpp"
#include "utils/animation.hpp"
#include "entities/power_up.hpp"

class SineWaveFlyer : public Entity
{
public:
    /**
     * Create a sine-wave flyer
     *
     * @param pos
     * @param power_up_type the type of power-up the flyer can drop
     */
    SineWaveFlyer(Vector2 pos_, PowerUpType power_up_type_);
    ~SineWaveFlyer();

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;

private:
    PowerUpType power_up_type{PowerUpType::NONE};

    float origin{0};               // Original y-coordinate of flyer when spawned to use as center of sine wave
    const float max_height{20.0f}; // Maximum height the flyer can go from its origin
    int frame_counter{0};
    const float period{90.0f}; // Period of sine wave (in frames)
    const int update_x_time{2};
    const float x_vel{-3.0f}; // X-axis velocity of the flyer

    Animation *animation{nullptr};

    const unsigned int score{100};
};

#endif // SINE_WAVE_FLYER_HPP