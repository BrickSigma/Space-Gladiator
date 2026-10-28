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
    int current_scene = static_cast<int>(this->current_scene);
    this->current_scene = scenes[current_scene]->update();
}