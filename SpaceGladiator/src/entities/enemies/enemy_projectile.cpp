#include "entities/enemies/enemy_projectile.hpp"

#include "utils/texture_manager.hpp"

#include <cmath>

EnemyProjectile::EnemyProjectile(Vector2 pos_, float angle, float speed)
    : Projectile(EntityType::ENEMY_PROJECTILE, pos_, 6, 6, {0, 0})
{
    velocity.x = speed * cosf(angle);
    velocity.y = speed * sinf(angle);

    TextureManager &texture_manager = TextureManager::getInstance();
    animation = new Animation(texture_manager.enemy_bullet, 6, 6, true);
}

EnemyProjectile::~EnemyProjectile()
{
    delete animation;
}

void EnemyProjectile::draw()
{
    animation->draw(pos);
}