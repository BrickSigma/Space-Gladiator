#ifndef SCENE_HPP
#define SCENE_HPP

#include <raylib.h>

enum class SceneState : int;

class Scene
{
public:
    Scene() {};
    virtual ~Scene() = default;

    /**
     * Update function to be called in the game loop for the game.
     *
     * Takes in a pointer to the screen render texture to draw to.
     *
     * Returns the next scene state to process
     */
    virtual SceneState update(RenderTexture *screen) = 0;
};

#endif // SCENE_HPP