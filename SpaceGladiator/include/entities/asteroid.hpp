#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "entities/entity.hpp"

enum class AsteroidSize : int
{
    SMALL = 12,
    MEDIUM = 18,
    LARGE = 24
};

class Asteroid : public Entity
{
public:
    Asteroid(AsteroidSize size, Vector2 pos, Vector2 velocity);
    ~Asteroid() = default;

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(Entity *entity) override;

    AsteroidSize size;
    Vector2 velocity;
};

#endif // ASTEROID_HPP