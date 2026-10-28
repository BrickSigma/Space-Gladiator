#include "start_scene.hpp"
#include <raylib.h>

StartScene::StartScene() {}

StartScene::~StartScene() {}

SceneState StartScene::update(RenderTexture *screen)
{
    BeginTextureMode(*screen);
    ClearBackground(BLACK);
    DrawText("Space Gladiators", 40, 40, 12, WHITE);
    DrawText("Start", 100, 100, 8, RED);
    EndTextureMode();

    return SceneState::START_SCREEN;
}