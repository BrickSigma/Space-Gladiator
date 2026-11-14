#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "entities/entity.hpp"

/**
 * Generic projectile entity.
 *
 * The game has multiple projectiles, fired from either the player or enemies.
 * Projectiles have the following properties in general:
 *  - Position (Entity)
 *  - Size (Entity)
 *  - Velocity
 *
 * Projectiles also need to move, so we need a move function, which will lie in the update function.
 */
class Projectile : public Entity
{
public:
    Projectile(EntityType type_, Vector2 pos_, int width_, int height_, Vector2 velocity_);
    virtual ~Projectile() = default;

    // Move the projectile based on it's velocity. Also marks it for deletion if it goes off screen
    void update(GameScene *scene) override;

    // Draw the projectile to the screen. By default this will render a rectangle of the projectile
    void draw() override;

    // Projectiles shot should be able to collide with the tilemap
    void handleTilemapCollision(GameScene *scene) override;

    Vector2 velocity;
};

#endif // PROJECTILE_HPP