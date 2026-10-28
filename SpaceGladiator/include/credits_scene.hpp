#ifndef CREDITS_SCENE_HPP
#define CREDITS_SCENE_HPP

#include "scene_manager.hpp"

class CreditsScene : public Scene {
public:
    CreditsScene(RenderTexture *screen);
    ~CreditsScene();
    SceneState update();
};

#endif  // CREDITS_SCENE_HPP