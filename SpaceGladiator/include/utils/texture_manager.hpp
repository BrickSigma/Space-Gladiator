#ifndef TEXTURES_MANAGER_HPP
#define TEXTURES_MANAGER_HPP

#include <raylib.h>

class TextureManager
{
private:
    TextureManager(const TextureManager &) = delete;
    TextureManager &operator=(const TextureManager &) = delete;

    TextureManager();
    ~TextureManager();

public:
    // Get an instance of the singleton
    static TextureManager &getInstance();

    // Textures which are part of the texture manager

    Texture background;
    
    Texture player_normal;
    Texture player_up;
    Texture player_down;
    Texture player_explosion;

    Texture player_bullet;

    Texture asteroid_small;
    Texture asteroid_medium;
    Texture asteroid_large;

    Texture explosion_1_frames;
    Texture explosion_2_frames;

    Texture regular_diagonal_flyer;
    Texture special_diagonal_flyer;

    Texture regular_sine_wave_flyer;
    Texture special_sine_wave_flyer;

    Texture spinning_laser_base;

    Texture enemy_bullet;

    Texture moving_laser;
    Texture moving_laser_hole;
    Texture laser_loading;

    Texture regular_flyer_spawner;
    Texture special_flyer_spawner;
    Texture flyer;

    Texture canon;
};

#endif // TEXTURES_MANAGER_HPP