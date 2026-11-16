#ifndef SNAKE_FIRE_HPP
#define SNAKE_FIRE_HPP

#include "entities/projectile.hpp"

constexpr float SNAKE_FIRE_SPEED{1.0f};

class SnakeFire : public Projectile
{
public:
    SnakeFire(Vector2 pos_, float angle);
    ~SnakeFire() = default;

    void draw() override;
private:
    Texture texture;
};

#endif  // SNAKE_FIRE_HPP