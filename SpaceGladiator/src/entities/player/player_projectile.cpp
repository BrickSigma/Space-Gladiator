#include "entities/player/player_projectile.hpp"

PlayerProjectile::PlayerProjectile(PlayerProjectileType type_, Vector2 pos_, int width_, int height_, Vector2 velocity_)
    : Projectile(EntityType::PLAYER_PROJECTILE, pos_, width_, height_, velocity_), type(type_) {}

void PlayerProjectile::handleDestruction()
{
    delete_entity = true;
}