#include "utils/texture_manager.hpp"

#include <iostream>

static char *getImageAssetFile(const char *file)
{
    const char *strings[] = {
        "assets/images/",
        file};
    return TextJoin(const_cast<char **>(strings), 2, "");
}

TextureManager &TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

void TextureManager::renderText(const std::string &str, Vector2 pos) const
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
            continue;

        float index = static_cast<float>(str[i] - 'A');
        if (index < 0 || index >= 26)
            throw std::runtime_error("String is invalid! Make sure it is in uppercase and has no special symbols");

        DrawTextureRec(letters, {8.0f * index, 0, 8, 8}, {pos.x + 8.0f * static_cast<float>(i), pos.y}, WHITE);
    }
}

TextureManager::TextureManager()
{
    TraceLog(LOG_INFO, "Creating texture manager instance");

    start_screen = LoadTexture(getImageAssetFile("start-screen.png"));

    // Fonts and HUD
    numbers = LoadTexture(getImageAssetFile("numbers.png"));
    letters = LoadTexture(getImageAssetFile("letters.png"));
    hud = LoadTexture(getImageAssetFile("hud.png"));

    // Player related textures
    player_normal = LoadTexture(getImageAssetFile("player-normal.png"));
    player_down = LoadTexture(getImageAssetFile("player-down.png"));
    player_up = LoadTexture(getImageAssetFile("player-up.png"));
    player_explosion = LoadTexture(getImageAssetFile("player-explosion.png"));

    player_bullet = LoadTexture(getImageAssetFile("player-projectile.png"));
    player_multishot = LoadTexture(getImageAssetFile("player-multishot.png"));
    shield_health_bar = LoadTexture(getImageAssetFile("shield-health-bar.png"));
    boss_health_bar = LoadTexture(getImageAssetFile("boss-health-bar.png"));

    // Asteroid textures
    asteroid_small = LoadTexture(getImageAssetFile("asteroid-small.png"));
    asteroid_medium = LoadTexture(getImageAssetFile("asteroid-medium.png"));
    asteroid_large = LoadTexture(getImageAssetFile("asteroid-large.png"));

    // Diagonal flyer textures
    regular_diagonal_flyer = LoadTexture(getImageAssetFile("regular-diagonal-flyer.png"));
    special_diagonal_flyer = LoadTexture(getImageAssetFile("special-diagonal-flyer.png"));

    // Sine wave flyer textures
    regular_sine_wave_flyer = LoadTexture(getImageAssetFile("regular-sine-wave-flyer.png"));
    special_sine_wave_flyer = LoadTexture(getImageAssetFile("special-sine-wave-flyer.png"));

    // Explosion frames
    explosion_1_frames = LoadTexture(getImageAssetFile("explosion-1.png"));
    explosion_2_frames = LoadTexture(getImageAssetFile("explosion-2.png"));
    giant_explosion = LoadTexture(getImageAssetFile("giant-explosion.png"));

    // Spining laser base texture
    spinning_laser_base = LoadTexture(getImageAssetFile("spinning-laser-base.png"));

    // Enemy bullet
    enemy_bullet = LoadTexture(getImageAssetFile("enemy-bullet.png"));

    // Moving laser textures
    moving_laser = LoadTexture(getImageAssetFile("moving-laser.png"));
    moving_laser_hole = LoadTexture(getImageAssetFile("moving-laser-hole.png"));
    laser_loading = LoadTexture(getImageAssetFile("moving-laser-loading.png"));

    // Flyer spawner textures
    regular_flyer_spawner = LoadTexture(getImageAssetFile("regular-flyer-spawner.png"));
    special_flyer_spawner = LoadTexture(getImageAssetFile("special-flyer-spawner.png"));
    flyer = LoadTexture(getImageAssetFile("flyer.png"));

    // Canon texture
    canon = LoadTexture(getImageAssetFile("canon.png"));

    // Ground shooter textures
    regular_ground_shooter = LoadTexture(getImageAssetFile("regular-ground-shooter.png"));
    special_ground_shooter = LoadTexture(getImageAssetFile("special-ground-shooter.png"));

    // Power-ups texture
    power_ups = LoadTexture(getImageAssetFile("power-ups.png"));

    // Blockade ball texture
    blockade_ball = LoadTexture(getImageAssetFile("blockade-ball.png"));

    // Big spaceship textures
    big_spaceship = LoadTexture(getImageAssetFile("big-spaceship.png"));
    big_spaceship_bullet = LoadTexture(getImageAssetFile("big-spaceship-bullet.png"));

    // Snake textures
    snake_head = LoadTexture(getImageAssetFile("snake-head.png"));
    snake_spine = LoadTexture(getImageAssetFile("snake-spine.png"));
    snake_fire = LoadTexture(getImageAssetFile("snake-fire.png"));

    // Laser eye textures
    laser_eye = LoadTexture(getImageAssetFile("laser-eye.png"));
    laser_eye_ball = LoadTexture(getImageAssetFile("laser-eye-ball.png"));
    laser_eye_frames = LoadTexture(getImageAssetFile("laser-eye-frames.png"));
    laser_eye_back = LoadTexture(getImageAssetFile("laser-eye-back.png"));
}

TextureManager::~TextureManager()
{
    TraceLog(LOG_INFO, "Destroying texture manager instance");

    UnloadTexture(start_screen);

    // Fonts and HUD
    UnloadTexture(numbers);
    UnloadTexture(letters);
    UnloadTexture(hud);

    // Player textures
    UnloadTexture(player_normal);
    UnloadTexture(player_down);
    UnloadTexture(player_up);
    UnloadTexture(player_explosion);

    UnloadTexture(player_bullet);
    UnloadTexture(player_multishot);
    UnloadTexture(shield_health_bar);
    UnloadTexture(boss_health_bar);

    // Asteroid textures
    UnloadTexture(asteroid_small);
    UnloadTexture(asteroid_medium);
    UnloadTexture(asteroid_large);

    // Diagonal flyer textures
    UnloadTexture(regular_diagonal_flyer);
    UnloadTexture(special_diagonal_flyer);

    // Sine wave flyer textures
    UnloadTexture(regular_sine_wave_flyer);
    UnloadTexture(special_sine_wave_flyer);

    // Explosion textures
    UnloadTexture(explosion_1_frames);
    UnloadTexture(explosion_2_frames);
    UnloadTexture(giant_explosion);

    // Spinning laser base texture
    UnloadTexture(spinning_laser_base);

    // Enemy bullet
    UnloadTexture(enemy_bullet);

    // Moving laser textures
    UnloadTexture(moving_laser);
    UnloadTexture(moving_laser_hole);
    UnloadTexture(laser_loading);

    // Flyer spawner textures
    UnloadTexture(regular_flyer_spawner);
    UnloadTexture(special_flyer_spawner);
    UnloadTexture(flyer);

    // Canon texture
    UnloadTexture(canon);

    // Ground shooter textures
    UnloadTexture(regular_ground_shooter);
    UnloadTexture(special_ground_shooter);

    // Power-ups texture
    UnloadTexture(power_ups);

    // Blockade ball texture
    UnloadTexture(blockade_ball);

    // Big spaceship textures
    UnloadTexture(big_spaceship);
    UnloadTexture(big_spaceship_bullet);

    // Snake textures
    UnloadTexture(snake_head);
    UnloadTexture(snake_spine);
    UnloadTexture(snake_fire);

    // Laser eye textures
    UnloadTexture(laser_eye);
    UnloadTexture(laser_eye_ball);
    UnloadTexture(laser_eye_frames);
    UnloadTexture(laser_eye_back);
}