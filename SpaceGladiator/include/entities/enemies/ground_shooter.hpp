#ifndef GROUND_SHOOTER_HPP
#define GROUND_SHOOTER_HPP

#include "entities/entity.hpp"
#include "entities/power_up.hpp"

constexpr float SHOOTER_SPEED{1.0f};

class GroundShooter : public Entity
{
public:
    GroundShooter(Vector2 pos_, bool facing_up_, PowerUpType power_up_type_);
    ~GroundShooter() = default;

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;

private:
    int frame_counter{75};
    const int fire_time{160};

    float target_angle{0}; // The current angular position of the player relative to the shooter

    bool facing_up;
    PowerUpType power_up_type{PowerUpType::NONE};

    Texture sprites;

    const unsigned int score{100};
};

#endif // GROUND_SHOOTER_HPP