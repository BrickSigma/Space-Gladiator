#ifndef START_SCENE_HPP
#define START_SCENE_HPP

#include "scenes/scene_manager.hpp"

class StartScene : public Scene {
public:
    StartScene(RenderTexture *screen_);
    ~StartScene();
    SceneState update() override;

    // Used to start playing the title screen music and reset the current option
    void onEnter(SceneState previous_state) override;

private:
    Texture background;
    Texture player;

    int current_option{0};
};

#endif  // START_SCENE_HPP