#include "scene_manager.hpp"
#include "constants.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else
#define GLSL_VERSION 100
#endif

SceneManager::SceneManager(RenderTexture *screenTexture, Scene *start_scene, Scene *game_scene, Scene *credits_scene) : screen(screenTexture)
{
    // Setup the scenes
    scenes[0] = start_scene;
    scenes[1] = game_scene;
    scenes[2] = credits_scene;

    // Load the CRT shader
    crt_shader = LoadShader(0, TextFormat("assets/shaders/glsl%i/crt.frag", GLSL_VERSION));
    colDiffuseLoc = GetShaderLocation(crt_shader, "colDiffuse");
    timeLoc = GetShaderLocation(crt_shader, "time");

    float colDiffuse[4] = {1, 1, 1, 0};
    SetShaderValue(crt_shader, colDiffuseLoc, colDiffuse, SHADER_UNIFORM_VEC4);
}

SceneManager::~SceneManager()
{
    UnloadShader(crt_shader);
}

void SceneManager::update()
{
    int current_scene = static_cast<int>(this->current_state);
    SceneState next_state = scenes[current_scene]->update();

    // If the C-key is pressed, toggle the shader
    if (IsKeyPressed(KEY_C))
    {
        useShader = !useShader;
    }

    if (next_state == SceneState::NO_CHANGE)
        return;

    // Handle the scene exit and enter callbacks if there are any of them
    scenes[current_scene]->onExit(next_state);
    int next_scene = static_cast<int>(next_state);
    scenes[next_scene]->onEnter(this->current_state);

    this->current_state = next_state;
}

void SceneManager::draw()
{
    if (useShader)
    {
        // Update the shader time value
        float time = static_cast<float>(GetTime());
        SetShaderValue(crt_shader, timeLoc, &time, SHADER_UNIFORM_FLOAT);
        BeginShaderMode(crt_shader);
    }

    DrawTexturePro(screen->texture, {0, 0, static_cast<float>(CANVAS_WIDTH), static_cast<float>(-CANVAS_HEIGHT)}, {0, 0, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)}, {0, 0}, 0, WHITE);

    if (useShader)
        EndShaderMode();
}