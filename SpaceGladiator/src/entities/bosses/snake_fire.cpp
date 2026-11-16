#include "entities/bosses/snake_fire.hpp"

#include "utils/texture_manager.hpp"

#include <cmath>

SnakeFire::SnakeFire(Vector2 pos_, float angle)
    : Projectile(EntityType::ENEMY_PROJECTILE, pos_, 7, 6, {0, 0})
{
    velocity.x = SNAKE_FIRE_SPEED * cosf(angle);
    velocity.y = SNAKE_FIRE_SPEED * sinf(angle);

    TextureManager &texture_manager = TextureManager::getInstance();
    texture = texture_manager.snake_fire;
}

void SnakeFire::draw()
{
    DrawTexture(texture, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);
}