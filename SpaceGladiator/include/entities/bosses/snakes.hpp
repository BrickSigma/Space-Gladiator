#ifndef SNAKES_HPP
#define SNAKES_HPP

#include "entities/entity.hpp"

class Snake : public Entity
{
public:
    Snake(Vector2 base_, Vector2 head_origin_, Vector2 amplitude_, int period_offset, int fire_time_, int health_);
    ~Snake() = default;

    void update(GameScene *scene) override;
    void draw() override;
    // Handle the collision of an entity with the snake (specific to bullets)
    void handleCollision(GameScene *scene, Entity *entity) override;
    // Check if the snake has collided with an entity
    bool checkCollision(const Entity &entity) const override;

    int health{0}; // The health of the snake

private:
    Vector2 base{0, 0};
    Vector2 head_origin{0, 0};
    Vector2 amplitude{0, 0};
    int frame_counter{0};
    const float period{180.0f};

    int fire_time{0};         // Time delay between the snake firing
    bool mouth_open{false};   // Keep a track if the snake's mouth is open
    const int fire_delay{30}; // Open the mouth .5 seconds before firing

    static constexpr int SPINE_LEN{6}; // The length of the spine

    Texture snake_head;
    Texture snake_spine;

    Vector2 spine[SPINE_LEN]{};

    bool spawning{true}; // Used to check if the snake is still spawning into the screen

    const unsigned int score{5000};

    // Check if an entity has collided with the head of the snake (used for bullets)
    bool checkHeadCollision(const Entity &entity) const;

    // Check if an entity has collided with a part of the spine
    bool checkSpineCollision(const Entity &entity, Vector2 spine_pos) const;

    // Get the X position of the snake head
    float getXHead(int frame);

    // Get the Y position of the snake head
    float getYHead(int frame);
};

class Snakes : public Entity
{
public:
    Snakes();
    ~Snakes();

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;
    bool checkCollision(const Entity &entity) const override;

private:
    Snake *snakes[3]{nullptr};

    int frame_counter{0};
    const int death_time{150}; // Wait 2.5 seconds before moving to the next level
};

#endif // SNAKES_HPP