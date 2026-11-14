#ifndef PLAYER_BULLET_HPP
#define PLAYER_BULLET_HPP

#include "entities/player/player_projectile.hpp"
#include <raylib.h>

class PlayerBullet : public PlayerProjectile
{
public:
    PlayerBullet(Vector2 pos_, Vector2 velocity_);
    ~PlayerBullet() = default;

    void draw() override;
};

#endif // PLAYER_BULLET_HPP