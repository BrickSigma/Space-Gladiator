#ifndef SPIRAL_BULLET_HPP
#define SPIRAL_BULLET_HPP

#include "entities/projectile.hpp"
#include "utils/animation.hpp"

class SpiralBullet : public Projectile
{
public:
    SpiralBullet(Vector2 pos_, Vector2 origin_);
    ~SpiralBullet();

    void update(GameScene *scene) override;
    void draw() override;

    Animation *animation{nullptr};
    Vector2 origin;

private:
    const float angular_vel{1}; // Rotate 1 degrees every frame
    const float vel{1.0f};      // Move some units every frame
};

#endif // SPIRAL_BULLET_HPP