#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include <raylib.h>
#include "scene.hpp"

// List of possible scene types
enum class SceneState : int
{
    START_SCREEN, // Start screen state
    GAME,         // Main game
    CREDITS,      // End credits
};

// Scene manager, used to handle different scenes like the start
class SceneManager
{
public:
    SceneManager(RenderTexture *screen, Scene *start_scene, Scene *game_scene, Scene *credits_scene);
    ~SceneManager();

    // Main update function for the scene manager
    void update();

    // Pointer to the screen canvas (should be 256x224)
    RenderTexture *screen = nullptr;

private:
    // List of all the scenes in the scene manager
    Scene *scenes[3] = {};

    // The current scene being run
    SceneState current_scene = SceneState::START_SCREEN;
};

#endif // SCENE_MANAGER_HPP