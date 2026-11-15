#ifndef BIG_SPACESHIP_HPP
#define BIG_SPACESHIP_HPP

#include "entities/entity.hpp"
#include "utils/animation.hpp"

class BigSpaceship : public Entity
{
public:
    BigSpaceship();
    ~BigSpaceship();

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;
    bool checkCollision(const Entity &entity) const override;

private:
    enum class BigSpaceshipState
    {
        SPAWNING,
        MOVING,
        DEAD
    };

    BigSpaceshipState state{BigSpaceshipState::SPAWNING};
    int frame_counter{0};
    const float period{300};  // Period of the sine wave movement
    const int fire_time{150}; // Fire bullets every 2.5 seconds
    int health{16};           // The health of the boss
    const int death_time{150};  // Wait 2.5 seconds before moving to the next level

    unsigned int score{5000};

    Texture texture;
    Animation *explosion{nullptr};
};

#endif // BIG_SPACESHIP_HPP