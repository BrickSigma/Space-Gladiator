#include "player.hpp"
#include "game_scene.hpp"

#include "constants.hpp"

Player::Player(GameScene *scene) : Entity({24, (CANVAS_HEIGHT / 2) - 8}, 16, 16, scene)
{
    // Load up the player's assets and other components needed
}

Player::~Player()
{
    // Free any allocated assets
}

void Player::handleInput() {
}

void Player::draw(RenderTexture *screen) {

}

void Player::update()
{
    handleInput();
    draw(this->scene->screen);
}