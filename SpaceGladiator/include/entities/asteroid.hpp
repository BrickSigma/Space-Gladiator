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
    Asteroid(AsteroidSize size, Vector2 pos, int direction);
    ~Asteroid();

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;

    AsteroidSize size;
    Vector2 velocity;

private:
    // Frame-based timing variables
    int frame_counter{0};
    int update_x_time{1};
    int update_y_time{0};

    // Asteroid texture
    Texture texture;
};

#endif // ASTEROID_HPP