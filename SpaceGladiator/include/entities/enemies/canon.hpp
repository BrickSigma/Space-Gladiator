#ifndef CANON_HPP
#define CANON_HPP

#include "entities/entity.hpp"

constexpr float CANON_SPEED{1.3f};  // Speed of the bullets fired from the canon

class Canon : public Entity
{
public:
    /**
     * Create a canon.
     *
     * @param pos_
     * @param facing_up_ when `true` the canon is facing upwards
     */
    Canon(Vector2 pos_, bool facing_up_);
    ~Canon() = default;

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;

private:
    int frame_counter{60};
    bool facing_up{false};
    Texture texture;
    const int fire_time{120}; // Fire every 2 seconds
};

#endif // CANON_HPP