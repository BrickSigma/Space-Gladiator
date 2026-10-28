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

    // Pointer to screen to render to for the scene
    RenderTexture *screen = nullptr;
};

#endif // SCENE_HPP