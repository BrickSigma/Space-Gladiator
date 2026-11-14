#include "entities/player/player_laser.hpp"

PlayerLaser::PlayerLaser(Vector2 pos_) : PlayerProjectile(PlayerProjectileType::LASER, pos_, 16, 2, {6, 0}) {}

void PlayerLaser::handleDestruction() {
    // Do nothing
}

void PlayerLaser::draw()
{
    DrawRectangle(static_cast<int>(pos.x), static_cast<int>(pos.y), width, height, PLAYER_LASER_COLOR);
}