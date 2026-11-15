#include "entities/bosses/big_spaceship_bullet.hpp"

#include "utils/texture_manager.hpp"

BigSpaceshipBullet::BigSpaceshipBullet(Vector2 pos_)
: Projectile(EntityType::ENEMY_PROJECTILE, pos_, 16, 4, {-4.0f, 0.0f})
{
    TextureManager &texture_manager = TextureManager::getInstance();
    texture = texture_manager.big_spaceship_bullet;
}

void BigSpaceshipBullet::draw()
{
    DrawTexture(texture, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);
}