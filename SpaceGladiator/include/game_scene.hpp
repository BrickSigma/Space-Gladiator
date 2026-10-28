#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "scene_manager.hpp"

class GameScene : public Scene {
public:
    GameScene();
    ~GameScene();
    SceneState update(RenderTexture *screen);

private:
};

#endif  // GAME_SCENE_HPP