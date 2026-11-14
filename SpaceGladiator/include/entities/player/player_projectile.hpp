#ifndef PLAYER_PROJECTILE_HPP
#define PLAYER_PROJECTILE_HPP

#include "entities/projectile.hpp"

// The different projectiles a player can fire, used for filtering during collision detection
enum class PlayerProjectileType
{
    BULLET,
    MULTISHOT,
    LASER
};

/**
 * Class used to represent all projectiles shot by the player, including:
 *  - regular bullets,
 *  - multi-shot bullets,
 *  - laser bullets
 */
class PlayerProjectile : public Projectile
{
public:
    PlayerProjectile(PlayerProjectileType type_, Vector2 pos_, int width_, int height_, Vector2 velocity_);
    virtual ~PlayerProjectile() = default;

    // Allow the projectile to handle if it get's destroyed or not. Mostly needed by the laser projectile
    virtual void handleDestruction();

    PlayerProjectileType type{PlayerProjectileType::BULLET};
};

#endif // PLAYER_PROJECTILE_HPP