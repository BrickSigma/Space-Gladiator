#ifndef POWER_UP_HPP
#define POWER_UP_HPP

#include "entities/entity.hpp"

enum class PowerUpType
{
    NONE,
    SPEED,
    MULTISHOT,
    LASER,
    SHIELD,
    EXTRA_SHOT
};

class PowerUp : public Entity
{
public:
    PowerUp(PowerUpType type_, Vector2 pos_);
    ~PowerUp() = default;

    void update(GameScene *scene) override;
    void draw() override;

    PowerUpType type{PowerUpType::NONE};

private:
    Texture texture;
    float sprite_index{0.0f};
    int frame_counter{0};
};

#endif // POWER_UP_HPP