#include "utils/texture_manager.hpp"

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

TextureManager::TextureManager()
{
    TraceLog(LOG_INFO, "Creating texture manager instance");
    background = LoadTexture(getImageAssetFile("background.png"));

    // Player related textures
    player_normal = LoadTexture(getImageAssetFile("player-normal.png"));
    player_down = LoadTexture(getImageAssetFile("player-down.png"));
    player_up = LoadTexture(getImageAssetFile("player-up.png"));
    player_explosion = LoadTexture(getImageAssetFile("player-explosion.png"));

    player_bullet = LoadTexture(getImageAssetFile("player-projectile.png"));

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

    // Spining laser base texture
    spinning_laser_base = LoadTexture(getImageAssetFile("spinning-laser-base.png"));

    // Enemy bullet
    enemy_bullet = LoadTexture(getImageAssetFile("enemy-bullet.png"));
}

TextureManager::~TextureManager()
{
    TraceLog(LOG_INFO, "Destroying texture manager instance");

    UnloadTexture(background);

    // Player textures
    UnloadTexture(player_normal);
    UnloadTexture(player_down);
    UnloadTexture(player_up);
    UnloadTexture(player_explosion);

    UnloadTexture(player_bullet);

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

    // Spinning laser base texture
    UnloadTexture(spinning_laser_base);

    // Enemy bullet
    UnloadTexture(enemy_bullet);
}