#ifndef DIAGONAL_FLYER_HPP
#define DIAGONAL_FLYER_HPP

#include "entities/entity.hpp"
#include "entities/power_up.hpp"

class DiagonalFlyer : public Entity
{
public:
    /**
     * Create a diagonal flyer.
     * 
     * @param pos
     * @param power_up_ the type of power-up the flyer can drop
     */
    DiagonalFlyer(Vector2 pos_,  PowerUpType power_up_);
    ~DiagonalFlyer() = default;

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;

private:
    enum DiagonalFlyerState
    {
        MOVING_DOWN,
        MOVING_UP,
        MOVING_STRAIGHT
    };

    PowerUpType power_up_type{PowerUpType::NONE};

    int frame_counter{0};
    int update_x_time{1};
    int update_y_time{1};

    // The current state of the flyer
    DiagonalFlyer::DiagonalFlyerState state{DiagonalFlyer::DiagonalFlyerState::MOVING_STRAIGHT};

    Vector2 velocity{-1, 0};

    // Store the sprite sheet for the flyer
    Texture textures;
};

#endif // DIAGONAL_FLYER_HPP