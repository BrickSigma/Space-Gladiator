#include "entities/projectile.hpp"

Projectile::Projectile(EntityType type, Vector2 pos, int width, int height, Vector2 velocity) : Entity(type, pos, width, height), velocity(velocity) {}

void Projectile::update(GameScene *scene)
{
    (void)scene;
    pos.x += velocity.x;
    pos.y += velocity.y;

    // Check if the projectile is off screen and automatically mark it for deletion
    if (isOffScreen())
    {
        delete_entity = true;
    }
}

void Projectile::draw()
{
    DrawRectangle(static_cast<int>(pos.x), static_cast<int>(pos.y), width, height, RED);
}