#ifndef FLYER_SPAWNER_HPP
#define FLYER_SPAWNER_HPP

#include "entities/entity.hpp"
#include "utils/animation.hpp"

class Flyer : public Entity
{
public:
    Flyer(Vector2 pos_, float target_y_, bool flipped_);
    ~Flyer();

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;

private:
    enum class FlyerState
    {
        MOVING_VERTICALLY,
        MOVING_LEFT
    };

    int frame_counter{0};
    FlyerState state{FlyerState::MOVING_VERTICALLY};
    Animation *animation{nullptr};
    float target_y{0};

    bool flipped{false};

    const unsigned int score{100};
};

class FlyerSpawner : public Entity
{
public:
    FlyerSpawner(Vector2 pos_, bool flipped_);
    ~FlyerSpawner() = default;

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;

private:
    static constexpr int WIDTH{32};
    static constexpr int HEIGHT{27};

    enum class FlyerSpawnerState
    {
        WAITING,
        SPAWNING
    };

    bool flipped{false}; // When `true`, the flyer is upside down

    int frame_counter{50};
    const int wait_duration{60}; // Wait for 2 seconds
    const int spawn_delay{25};   // Spawn a new flyer every second
    int flyers_spawned{0};       // The number of flyers spawned
    const int max_flyers{4};     // Maximum number of flyers that may be spawned
    int health{3};               // It takes 3 bullet hits to destroy the spawner

    FlyerSpawnerState state{FlyerSpawnerState::WAITING};

    Texture texture;

    const unsigned int score{1000};
};

#endif // FLYER_SPAWNER_HPP