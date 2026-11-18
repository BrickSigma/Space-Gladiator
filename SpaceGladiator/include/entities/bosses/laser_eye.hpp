#ifndef LASER_EYE_HPP
#define LASER_EYE_HPP

#include "entities/entity.hpp"
#include "utils/animation.hpp"

class LaserEye : public Entity
{
public:
    LaserEye(Vector2 pos_, RenderTexture canvas_);
    ~LaserEye();

    void update(GameScene *scene) override;
    void draw() override;
    void handleCollision(GameScene *scene, Entity *entity) override;
    bool checkCollision(const Entity &entity) const override;

private:
    enum class LaserEyeState
    {
        DELAY,
        SPAWNING,
        WAITING,
        ATTACK_1,
        ATTACK_2,
        ATTACK_3,
        ATTACK_4
    };

    static constexpr int WIDTH{64};
    static constexpr int HEIGHT{56};

    LaserEyeState current_state{LaserEyeState::DELAY};
    LaserEyeState next_state{LaserEyeState::ATTACK_1};

    int frame_counter{0};
    const int delay_time{360};    // Wait 6 seconds after spawning
    const int wait_time{30};      // Wait for 0.5 second (should be enough time to hit the eye)
    const int attack_1_time{120}; // Attack 1 lasts 2 seconds
    const int attack_2_time{240}; // Attack 2 lasts 3 seconds
    const int attack_3_time{300}; // Attack 3 lasts 5 seconds
    const int attack_4_time{120}; // Attack 4 lasts 2 seconds
    const int death_time{90};     // Wait 1.5 seconds once the laser eye is dead

    Vector2 center{0.0f, 0.0f}; // The center of the eye

    Texture laser_eye;
    Texture eye_ball;
    Texture background;
    Animation *frames{nullptr};
    Animation *explosion{nullptr};
    RenderTexture eye_canvas; // Used for masking the eye during rendering
    Texture mask;
    RenderTexture canvas; // This is used to go back to rendering to the screen when rendering to the eye canvas

    Shader mask_shader;
    int mask_loc;

    void attack1(GameScene *scene); // Attack 1 pattern
    void attack2(GameScene *scene); // Attack 2 pattern
    void attack3(GameScene *scene); // Attack 3 pattern
    void attack4(GameScene *scene); // Attack 4 pattern

    int health{16};

    const unsigned int score{10000};

    float player_angle{0};              // Used to keep track of the player's angle
    float player_distance{0};           // The distance of the player from the eye's center
    const float distance_clamp{100.0f}; // Max distance of player when clamping eye position

    // Attack 1 specific stuff
    const int attack_1_fire_time{45};

    // Attack 2 specific stuff
    const int attack_2_fire_time{20};

    // Attack 3 specific stuff
    const int attack_3_fire_time{15};
    const float attack_3_angular_vel{1};
    float attack_3_angle{0}; // The attack rotates over time as the spiral is created

    // Attack 4 specific stuff
    const int attack_4_fire_time{8};
};

#endif // LASER_EYE_HPP