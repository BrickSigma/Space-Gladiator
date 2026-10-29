#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

#define PLAYER_BASE_SPEED (1)

class Player : public Entity
{
public:
    Player(GameScene *scene);
    ~Player();

    // Reset the player's state
    void reset();

    // Update the player
    void update() override;

private:
    // Handle user input to move the player or shoot
    void handleInput();

    // Handle the physics of moving
    void handlePhysics();

    // Draw the space ship to the screen
    void draw() override;

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

    enum PlayerTextureDirection
    {
        PLAYER_TEXTURE_NORMAL,
        PLAYER_TEXTURE_DOWN,
        PLAYER_TEXTURE_UP
    };

    Texture textures[3] = {};
};

#endif // PLAYER_HPP