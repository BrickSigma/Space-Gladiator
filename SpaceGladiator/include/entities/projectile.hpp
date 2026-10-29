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
    Projectile(EntityType type, Vector2 pos, int width, int height, Vector2 velocity);
    ~Projectile() = 0;

    // Move the projectile based on it's velocity. Also marks it for deletion if it goes off screen
    void update(GameScene *scene) override;

    // Check if the projectile is off screen. Ruturns true if it is
    virtual bool isOffScreen();

    void draw() override = 0;

    Vector2 velocity;
};

#endif // PROJECTILE_HPP