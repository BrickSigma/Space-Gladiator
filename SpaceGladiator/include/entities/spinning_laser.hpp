#ifndef SPINNING_LASER_HPP
#define SPINNING_LASER_HPP

#include "entities/entity.hpp"
#include "utils/animation.hpp"

#include <numbers>
#include <vector>

class LaserBall : public Entity
{
public:
    LaserBall(Vector2 pos, Vector2 rotation_origin, int rotation_direction);
    ~LaserBall();

    void update(GameScene *scene) override;
    void draw() override;
    
    Vector2 origin; // The center of the spinning laser
private:
    int direction{1};
    Animation *bullet{nullptr};
    const float angle{2.0f * (std::numbers::pi_v<float> / 180.0f)}; // The rotational speed of the ball, in radians/frame
};

enum class SpinningLaserType
{
    LEFT,
    RIGHT,
    ALTERNATE_LEFT,
    ALTERNATE_RIGHT
};

class SpinningLaser : public Entity
{
public:
    /**
     * Create a new spinning laser enemy
     *
     * @param pos
     * @param no_lasers the number of "lasers" to add to the entity
     * @param type
     */
    SpinningLaser(Vector2 pos, int no_lasers, SpinningLaserType type);
    ~SpinningLaser();

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;

    // Special check collision function used when checking if we're hitting the player
    bool checkCollision(const Entity& entity) const override;

private:
    const float spacing{16.0f};
    std::vector<LaserBall *> laser_balls{};
    Texture texture;

    int frame_counter{0};
};

#endif // SPINNING_LASER_HPP