#ifndef PLAYER_LASER_HPP
#define PLAYER_LASER_HPP

#include "entities/player/player_projectile.hpp"

constexpr Color PLAYER_LASER_COLOR{4, 248, 252, 255};

class PlayerLaser : public PlayerProjectile
{
public:
    PlayerLaser(Vector2 pos_);
    ~PlayerLaser() = default;

    void handleDestruction() override;
    void draw() override;
};

#endif // PLAYER_LASER_HPP