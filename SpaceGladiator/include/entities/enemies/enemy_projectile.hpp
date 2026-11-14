#ifndef ENEMY_PROJECTILE_HPP
#define ENEMY_PROJECTILE_HPP

#include "entities/projectile.hpp"
#include "utils/animation.hpp"

/**
 * Enemy projectile/bullet object which is directed towards a player
 */
class EnemyProjectile : public Projectile
{
public:
    EnemyProjectile(Vector2 pos_, float angle, float speed);
    ~EnemyProjectile();

    void draw() override;
private:
    Animation *animation{nullptr};
};

#endif // ENEMY_PROJECTILE_HPP