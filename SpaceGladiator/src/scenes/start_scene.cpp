#include "scenes/start_scene.hpp"
#include <raylib.h>

#include "utils/texture_manager.hpp"
#include "utils/audio_manager.hpp"
#include "utils/constants.hpp"

StartScene::StartScene(RenderTexture *screen_) : Scene(screen_)
{
    TextureManager &texture_manager = TextureManager::getInstance();
    background = texture_manager.start_screen;
    player = texture_manager.player_normal;
}

StartScene::~StartScene() {}

SceneState StartScene::update()
{
    SceneState next_state = SceneState::NO_CHANGE;

    // Handle user input
    if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
    {
        if (current_option == 0)
            next_state = SceneState::GAME;
        else
            next_state = SceneState::CREDITS;
    }

    // Todo: allow controller to use menu
    if (IsKeyPressed(KEY_DOWN))
        current_option = 1;
    else if (IsKeyPressed(KEY_UP))
        current_option = 0;

    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        if (IsKeyPressed(KEY_ZERO))
        {
            LEVEL_NO = 0;
            CHECKPOINT_NO = 0;
        }
        else if (IsKeyPressed(KEY_ONE))
        {
            LEVEL_NO = 0;
            CHECKPOINT_NO = 3;
        }
        else if (IsKeyPressed(KEY_TWO))
        {
            LEVEL_NO = 1;
            CHECKPOINT_NO = 3;
        }
        else if (IsKeyPressed(KEY_THREE))
        {
            LEVEL_NO = 2;
            CHECKPOINT_NO = 3;
        }
    }

    // Handle rendering here
    BeginTextureMode(*screen);
    DrawTexture(background, 0, 0, WHITE);

    DrawTexture(player, 72, 62 + 24 * current_option, WHITE);

    EndTextureMode();

    return next_state;
}

void StartScene::onEnter(SceneState previous_state)
{
    (void)previous_state;
    current_option = 0;

    AudioManager &audio_manager = AudioManager::getInstance();
    audio_manager.startMusicFile(MusicFile::START_SCREEN);
}