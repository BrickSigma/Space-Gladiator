#ifndef SCENE_HPP
#define SCENE_HPP

#include <raylib.h>

enum class SceneState : int;

class Scene
{
public:
    Scene(RenderTexture *screen) : screen(screen) {};
    virtual ~Scene() = default;

    /**
     * Update function to be called in the game loop for the game.
     *
     * Returns the next scene state to process
     */
    virtual SceneState update() = 0;

    // Enter scene callback function, used to handle cases where the scene may have changed and what to do
    virtual void onEnter(SceneState previous_scene) {
        (void)previous_scene;
    };

    // Exit scene callback function, used to handle what to do when we exit the scene
    virtual void onExit(SceneState next_scene) {
        (void)next_scene;
    };

    // Pointer to screen to render to for the scene
    RenderTexture *screen = nullptr;
};

#endif // SCENE_HPP