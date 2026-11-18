#ifndef TEXTURES_MANAGER_HPP
#define TEXTURES_MANAGER_HPP

#include <raylib.h>
#include <string>

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

    // Render a string of text
    void renderText(const std::string& str, Vector2 pos) const;

    // Textures which are part of the texture manager

    Texture start_screen;
    Texture credits_screen;

    Texture numbers;
    Texture letters;
    Texture hud;
    
    Texture player_normal;
    Texture player_up;
    Texture player_down;
    Texture player_explosion;

    Texture player_bullet;
    Texture player_multishot;
    Texture shield_health_bar;
    Texture boss_health_bar;

    Texture asteroid_small;
    Texture asteroid_medium;
    Texture asteroid_large;

    Texture explosion_1_frames;
    Texture explosion_2_frames;
    Texture giant_explosion;
    Texture giant_explosion_2;

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

    Texture regular_ground_shooter;
    Texture special_ground_shooter;

    Texture power_ups;

    Texture blockade_ball;

    Texture big_spaceship;
    Texture big_spaceship_bullet;

    Texture snake_head;
    Texture snake_spine;
    Texture snake_fire;

    Texture laser_eye;
    Texture laser_eye_ball;
    Texture laser_eye_frames;
    Texture laser_eye_back;
    Texture laser_eye_mask;
};

#endif // TEXTURES_MANAGER_HPP