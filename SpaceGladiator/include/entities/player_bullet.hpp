#ifndef PLAYER_BULLET_HPP
#define PLAYER_BULLET_HPP

#include "entities/projectile.hpp"
#include <raylib.h>

class PlayerBullet : public Projectile
{
public:
    PlayerBullet(Vector2 pos, Vector2 velocity);
    ~PlayerBullet() = default;

    void draw() override;
};

#endif // PLAYER_BULLET_HPP