#ifndef MOVING_LASER_HPP
#define MOVING_LASER_HPP

#include "entities/entity.hpp"
#include "utils/animation.hpp"

constexpr float MOVING_LASER_WIDTH = 48;
constexpr float MOVING_LASER_HEIGHT = 32;

class MovingLaser : public Entity
{
public:
    MovingLaser();
    ~MovingLaser();

    void update(GameScene *scene) override;
    void draw() override;
    bool checkCollision(const Entity &entity) const override;

private:
    // The state of the moving laser
    enum class MovingLaserState {
        SPAWNING,
        MOVING,
        TARGETING,
        LOADING,
        FIRING,
        RETRACTING
    };

    MovingLaserState state{MovingLaserState::SPAWNING};
    int frame_counter{0};
    const int move_duration{180};  // Time spent oscillating up and down
    const float period{210};  // The period of the sine wave for moving the laser
    const int load_duration{30};  // The time spent loading the laser once it's locked on
    const int firing_duration{90};

    float player_target_y{0};  // The y-coordinate of the player used to lock onto
    int target_direction{1};  // Either `1` for moving down and `-1` for moving up towards the player when locking on

    Animation *laser_animation{nullptr};
    Texture moving_laser_hole;
    Animation *laser_loading_animation{nullptr};

    // Get the rectangle for the laser when it's fired
    Rectangle getLaserRect() const;
};

#endif // MOVING_LASER_HPP