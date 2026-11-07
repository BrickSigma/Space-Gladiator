#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entities/entity.hpp"

constexpr int PLAYER_BASE_SPEED = 1;
constexpr int PLAYER_MAX_PROJECTILES = 2; // Maximum number of projectiles the player can shoot
constexpr int SHOOT_DELAY = 10;           // Shoot every .166 seconds
constexpr int PROJECTILE_SPEED = 3;       // Speed of projectiles shot

constexpr int PLAYER_SPRITE_WIDTH = 26;
constexpr int PLAYER_SPRITE_HEIGHT = 12;
constexpr int PLAYER_HITBOX_WIDTH = 16;
constexpr int PLAYER_HITBOX_HEIGHT = 6;

constexpr int PLAYER_MAX_LIVES = 3;

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
    unsigned int speed = PLAYER_BASE_SPEED;

    // Texture related items for the player
    enum PlayerTextureDirection
    {
        PLAYER_TEXTURE_NORMAL,
        PLAYER_TEXTURE_DOWN,
        PLAYER_TEXTURE_UP
    };

    Texture textures[3] = {};

    // Projectiles currently shot by the player
    bool is_shooting = false;
    int shooting_timer = 0; // Timer tracking time since last shot
};

#endif // PLAYER_HPP