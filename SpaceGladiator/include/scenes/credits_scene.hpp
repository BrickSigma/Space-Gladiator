#ifndef CREDITS_SCENE_HPP
#define CREDITS_SCENE_HPP

#include "scenes/scene_manager.hpp"

class CreditsScene : public Scene {
public:
    CreditsScene(RenderTexture *screen_);
    ~CreditsScene();
    SceneState update() override;
};

#endif  // CREDITS_SCENE_HPP