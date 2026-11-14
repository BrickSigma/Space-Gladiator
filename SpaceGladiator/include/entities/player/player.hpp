#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entities/entity.hpp"
#include "utils/animation.hpp"
#include "entities/power_up.hpp"

constexpr float PLAYER_BASE_SPEED{1.0f};
constexpr int PLAYER_MAX_PROJECTILES{2};    // Maximum number of projectiles the player can shoot
constexpr int BASE_SHOOT_DELAY{10};         // Shoot every .166 seconds
constexpr int PROJECTILE_SPEED{3};          // Speed of projectiles shot
constexpr int PLAYER_SHIELD_MAX_HEALTH{12}; // Maximum strength of the player's sheild

constexpr int PLAYER_SPRITE_WIDTH{26};
constexpr int PLAYER_SPRITE_HEIGHT{12};
constexpr int PLAYER_HITBOX_WIDTH{16};
constexpr int PLAYER_HITBOX_HEIGHT{6};

constexpr int PLAYER_MAX_LIVES{3};

class Player : public Entity
{
public:
    Player();
    ~Player();

    // Reset the player's state. `restore_health` indicates whether we should set the lives back to max.
    void reset(bool restore_health);

    // Update the player
    void update(GameScene *scene) override;

    // Draw the space ship to the screen
    void draw() override;

    // Handle collisions with the space ship
    void handleCollision(GameScene *scene, Entity *entity) override;

    // Handle collisions with the tilemap
    void handleTilemapCollision(GameScene *scene) override;

    int lives = PLAYER_MAX_LIVES; // The amount of lives the player has remaining

private:
    // Handle user input to move the player or shoot
    void handleInput();
    // Handle the physics of moving
    void handlePhysics();
    // Handle the player shooting
    void handleShooting(GameScene *scene);

    // Set the movement array to false for all entries
    void clearMovementArray();

    // Handle activating a powerup when collected
    void handlePowerUp(PowerUp *power_up);

    // Player movement related data
    enum PlayerMovement
    {
        PLAYER_UP,
        PLAYER_DOWN,
        PLAYER_LEFT,
        PLAYER_RIGHT
    };

    // Used to indicate how the player moved based on input
    bool movement[4] = {0};

    // Player move speed
    float speed = PLAYER_BASE_SPEED;

    // Texture related items for the player
    enum PlayerTextureDirection
    {
        PLAYER_TEXTURE_NORMAL,
        PLAYER_TEXTURE_DOWN,
        PLAYER_TEXTURE_UP
    };

    Texture textures[3] = {};

    // Projectiles currently shot by the player
    bool is_shooting{false};
    size_t max_shots{2};   // The maximum number of shots a player can take at a time
    int shooting_timer{0}; // Timer tracking time since last shot
    int shoot_delay{BASE_SHOOT_DELAY};

    int frame_counter{0};          // Separate frame counter for other types of bullets, like the multishot
    bool has_laser{false};         // Used to indicate if the player has unlocked a laser gun
    bool has_multishot{false};     // Used to indicate if the player has multishot unlocked
    const int multishot_delay{90}; // Allow the multishot to trigger every 1.5 seconds

    // Shield health; when set to zero it means there is no shield active
    int shield{0};

    Animation *explosion{nullptr};
};

#endif // PLAYER_HPP