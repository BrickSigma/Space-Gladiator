#include "scene_manager.hpp"

SceneManager::SceneManager(RenderTexture *screen, Scene *start_scene, Scene *game_scene, Scene *credits_scene) : screen(screen)
{
    // Setup the scenes
    scenes[0] = start_scene;
    scenes[1] = game_scene;
    scenes[2] = credits_scene;
}

SceneManager::~SceneManager() {}

void SceneManager::update()
{
    int current_scene = static_cast<int>(this->current_state);
    SceneState next_state = scenes[current_scene]->update();

    if (next_state == SceneState::NO_CHANGE)
        return;

    // Handle the scene exit and enter callbacks if there are any of them
    scenes[current_scene]->onExit(next_state);
    int next_scene = static_cast<int>(next_state);
    scenes[next_scene]->onEnter(this->current_state);

    this->current_state = next_state;
}