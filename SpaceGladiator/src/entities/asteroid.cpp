#include "entities/asteroid.hpp"

#include "entities/player_bullet.hpp"

Asteroid::Asteroid(AsteroidSize size, Vector2 pos, Vector2 velocity)
    : Entity(EntityType::ENEMY, pos, static_cast<int>(size), static_cast<int>(size)),
      size(size),
      velocity(velocity) {}

void Asteroid::update(GameScene *scene)
{
    (void)scene;
    pos.x += velocity.x;
    pos.y += velocity.y;

    // Check if the asteroid is off screen and automatically mark it for deletion
    if (isOffScreen())
    {
        delete_entity = true;
    }
}

void Asteroid::draw()
{
    DrawRectangle(static_cast<int>(pos.x), static_cast<int>(pos.y), width, height, RED);
}

void Asteroid::handleCollision(Entity *entity) {
    if (entity->type != EntityType::PLAYER_PROJECTILE)
        return;

    if (checkCollision(*entity)) {
        delete_entity = true;
        entity->delete_entity = true;
    }
}