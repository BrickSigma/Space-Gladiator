#include "entities/asteroid.hpp"

#include "entities/player_bullet.hpp"
#include "scenes/game_scene.hpp"

Asteroid::Asteroid(AsteroidSize size, Vector2 pos, float direction)
    : Entity(EntityType::ENEMY, pos, static_cast<int>(size), static_cast<int>(size)),
      size(size)
{
    switch (size)
    {
    case AsteroidSize::SMALL:
        velocity = {-1, direction};
        update_time = 2;
        break;
    case AsteroidSize::MEDIUM:
        velocity = {-1, direction};
        update_time = 4;
        break;
    case AsteroidSize::LARGE:
        velocity = {-1, direction};
        update_time = 8;
        break;
    }
}

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

void Asteroid::handleCollision(GameScene *scene, Entity *entity)
{
    if (entity->type != EntityType::PLAYER_PROJECTILE)
        return;

    if (!checkCollision(*entity))
        return;

    // The player bullet and asteroid collided, so let's spawn in two more smaller asteroids
    delete_entity = true;
    entity->delete_entity = true;

    AsteroidSize next_size{AsteroidSize::MEDIUM};
    switch (size)
    {
    case AsteroidSize::LARGE:
        next_size = AsteroidSize::MEDIUM;
        break;
    case AsteroidSize::MEDIUM:
        next_size = AsteroidSize::SMALL;
        break;
    default:
        return;
    }

    scene->spawnEntity(new Asteroid(next_size, {pos.x, pos.y + height / 2 - static_cast<int>(next_size) / 2}, 0.25));
    scene->spawnEntity(new Asteroid(next_size, {pos.x, pos.y + height / 2 - static_cast<int>(next_size) / 2}, -0.25));
}