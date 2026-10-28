#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "scene_manager.hpp"
#include "entity.hpp"
#include "player.hpp"

#include <vector>

class GameScene : public Scene {
public:
    GameScene(RenderTexture *screen);
    ~GameScene();

    // Used to restart the game state from a fresh
    void reset();

    // Used to restart from a level, without resetting the score
    void resetFromCheckpoint();

    // Called in the update loop
    SceneState update() override;

    // Handle entering the game scene from another state
    void onEnter(SceneState previous_state) override;

    // Current distance into the level
    unsigned long distance = 0;

    // Current score
    unsigned long score = 0;

    // Last level succesfully reached
    unsigned int level = 1;
private:
    // Pointer to player object for quick access by other entities
    Player *player = nullptr;

    // List of all entities in the system;
    std::vector<Entity *> entities{};   
};

#endif  // GAME_SCENE_HPP