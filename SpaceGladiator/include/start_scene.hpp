#ifndef START_SCENE_HPP
#define START_SCENE_HPP

#include "scene_manager.hpp"

class StartScene : public Scene {
public:
    StartScene();
    ~StartScene();
    SceneState update(RenderTexture *screen);

private:
    // Pulse counter used for the start button
    unsigned int pulse = 0;
};

#endif  // START_SCENE_HPP