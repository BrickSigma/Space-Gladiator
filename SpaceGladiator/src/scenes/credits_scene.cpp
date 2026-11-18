#include "scenes/credits_scene.hpp"
#include <raylib.h>

#include "utils/audio_manager.hpp"
#include "utils/texture_manager.hpp"

CreditsScene::CreditsScene(RenderTexture *screen_) : Scene(screen_)
{
    background = TextureManager::getInstance().credits_screen;
}

CreditsScene::~CreditsScene() {}

SceneState CreditsScene::update()
{
    SceneState next_state = SceneState::NO_CHANGE;

    // Handle user input
    // if (IsKeyPressed(KEY_BACKSPACE))
    // {
    //     next_state = SceneState::GAME;
    // }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_BACKSPACE))
        next_state = SceneState::START_SCREEN;

    // Handle rendering here
    BeginTextureMode(*screen);
    DrawTexture(background, 0, 0, WHITE);
    EndTextureMode();

    return next_state;
}

void CreditsScene::onEnter(SceneState previous_state)
{
    (void)previous_state;
    AudioManager &audio_manager = AudioManager::getInstance();
    audio_manager.startMusicFile(MusicFile::CREDITS_SCREEN);
}