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
    SceneState update();

private:
    // Pointer to player object for quick access by other entities
    Player *player;

    // List of all entities in the system;
    std::vector<Entity *> entities{};   
};

#endif  // GAME_SCENE_HPP