#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entities/entity.hpp"

#define PLAYER_BASE_SPEED (1)
#define PLAYER_MAX_PROJECTILES (2) // Maximum number of projectiles the player can shoot
#define SHOOT_DELAY (15)           // Shoot every .333 seconds
#define PROJECTILE_SPEED (2)       // Speed of projectiles shot

class Player : public Entity
{
public:
    Player();
    ~Player();

    // Reset the player's state
    void reset();

    // Update the player
    void update(GameScene *scene) override;

private:
    // Handle user input to move the player or shoot
    void handleInput();
    // Handle the physics of moving
    void handlePhysics();
    // Handle the player shooting
    void handleShooting(GameScene *scene);

    // Draw the space ship to the screen
    void draw() override;

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