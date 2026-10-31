#ifndef START_SCENE_HPP
#define START_SCENE_HPP

#include "scene_manager.hpp"

class StartScene : public Scene {
public:
    StartScene(RenderTexture *screen);
    ~StartScene();
    SceneState update() override;
};

#endif  // START_SCENE_HPP