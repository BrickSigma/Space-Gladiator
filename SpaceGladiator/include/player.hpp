#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"

class Player : public Entity
{
public:
    Player(GameScene *scene);
    ~Player();

    // Update the player
    void update() override;

private:
    // Handle user input to move the player
    void handleInput();

    // Draw the space ship to the screen
    void draw(RenderTexture *screen);
};

#endif // PLAYER_HPP