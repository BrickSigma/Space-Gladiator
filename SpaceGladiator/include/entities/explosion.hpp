#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "entities/entity.hpp"
#include "utils/animation.hpp"

class Explosion : public Entity
{
public:
    Explosion(int type, Vector2 center);
    ~Explosion();

    void update(GameScene *scene);
    void draw();

    Animation *animation;
    int frame_counter{0};
    const int DISTANCE_UPDATE_TIMER{2};
};

#endif // EXPLOSION_HPP