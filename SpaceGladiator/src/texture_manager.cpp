#include "texture_manager.hpp"

TextureManager &TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

TextureManager::TextureManager()
{
    TraceLog(LOG_INFO, "Creating texture manager instance");
    player_normal = LoadTexture("assets/images/player-normal.png");
    player_down = LoadTexture("assets/images/player-down.png");
    player_up = LoadTexture("assets/images/player-up.png");
    player_bullet = LoadTexture("assets/images/player-projectile.png");
}

TextureManager::~TextureManager()
{
    TraceLog(LOG_INFO, "Destroying texture manager instance");
    UnloadTexture(player_normal);
    UnloadTexture(player_down);
    UnloadTexture(player_up);
    UnloadTexture(player_bullet);
}