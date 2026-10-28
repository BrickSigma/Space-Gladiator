#ifndef START_SCENE_HPP
#define START_SCENE_HPP

#include "scene_manager.hpp"

class StartScene : public Scene {
public:
    StartScene(RenderTexture *screen);
    ~StartScene();
    SceneState update() override;

private:
    // Pulse counter used for the start button
    unsigned int pulse = 0;
};

#endif  // START_SCENE_HPP