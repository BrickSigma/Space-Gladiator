#include "entities/projectile.hpp"

#include "scenes/game_scene.hpp"

Projectile::Projectile(EntityType type_, Vector2 pos_, int width_, int height_, Vector2 velocity_) : Entity(type_, pos_, width_, height_), velocity(velocity_) {}

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

void Projectile::handleTilemapCollision(GameScene *scene)
{
    if (delete_entity)
        return;

    if (!checkTilemapCollision(scene->level_manager.getTilemap(), scene->scroll_distance))
        return;

    delete_entity = true;
}