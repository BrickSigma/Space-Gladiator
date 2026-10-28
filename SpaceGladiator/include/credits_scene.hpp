#ifndef CREDITS_SCENE_HPP
#define CREDITS_SCENE_HPP

#include "scene_manager.hpp"

class CreditsScene : public Scene {
public:
    CreditsScene();
    ~CreditsScene();
    SceneState update(RenderTexture *screen);

private:
};

#endif  // CREDITS_SCENE_HPP