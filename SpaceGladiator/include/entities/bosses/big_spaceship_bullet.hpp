#ifndef BIG_SPACESHIP_BULLET_HPP
#define BIG_SPACESHIP_BULLET_HPP

#include "entities/projectile.hpp"


class BigSpaceshipBullet : public Projectile
{
public:
    BigSpaceshipBullet(Vector2 pos_);
    ~BigSpaceshipBullet() = default;

    void draw() override;

private:
    Texture texture;
};

#endif // BIG_SPACESHIP_BULLET_HPP