#include "entities/bosses/laser_eye.hpp"

#include "utils/texture_manager.hpp"
#include "scenes/game_scene.hpp"
#include "entities/enemies/enemy_projectile.hpp"
#include "entities/bosses/spiral_bullet.hpp"
#include "utils/math.hpp"
#include "utils/audio_manager.hpp"

#include <raymath.h>
#include <cmath>

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else
#define GLSL_VERSION 100
#endif

LaserEye::LaserEye(Vector2 pos_, RenderTexture canvas_) : Entity(EntityType::ENEMY, pos_, WIDTH, HEIGHT), canvas(canvas_)
{
    TextureManager &texture_manager = TextureManager::getInstance();
    laser_eye = texture_manager.laser_eye;
    eye_ball = texture_manager.laser_eye_ball;
    background = texture_manager.laser_eye_back;

    frames = new Animation(texture_manager.laser_eye_frames, 64, 5, false);
    center = {pos.x + 32.0f, pos.y + 32.0f};
    is_tile = true;

    explosion = new Animation(texture_manager.giant_explosion_2, 48, 8, false);

    eye_canvas = LoadRenderTexture(WIDTH, HEIGHT);
    mask = texture_manager.laser_eye_mask;

    mask_shader = LoadShader(0, TextFormat("assets/shaders/glsl%i/mask.frag", GLSL_VERSION));
    mask_loc = GetShaderLocation(mask_shader, "mask");
}

LaserEye::~LaserEye()
{
    delete frames;
    delete explosion;
    UnloadRenderTexture(eye_canvas);
    UnloadShader(mask_shader);
}

bool LaserEye::checkCollision(const Entity &entity) const
{
    if (is_dead)
        return false;

    Rectangle rect{pos.x + 2.0f, pos.y + 16.0f, 60, 32};
    if (CheckCollisionRecs(rect, entity.getRect()))
        return true;

    return false;
}

void LaserEye::handleCollision(GameScene *scene, Entity *entity)
{
    if (is_dead || current_state == LaserEyeState::DELAY || current_state == LaserEyeState::SPAWNING)
        return;

    if (entity->type != EntityType::PLAYER_PROJECTILE)
        return;

    if (!checkCollision(*entity))
        return;

    entity->delete_entity = true;
    scene->setCameraShake(CameraShake(2, 30));

    health--;
    scene->setBossHealth(health);
    if (health <= 0)
    {
        is_dead = true;
        frame_counter = 0;
        scene->addScore(score);
        scene->setCameraShake(CameraShake(3, 90));
        AudioManager::getInstance().playSFX(SFXFile::BOSS_EXPLOSION);
    }
}

void LaserEye::draw()
{
    if (is_dead)
    {
        explosion->draw({center.x - 24.0f, center.y - 24.0f});
        return;
    }

    if (current_state == LaserEyeState::DELAY)
    {
        DrawTexture(background, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);
        return;
    }
    else if (current_state == LaserEyeState::SPAWNING)
    {
        frames->draw(pos);
        return;
    }

    DrawTexture(laser_eye, static_cast<int>(pos.x), static_cast<int>(pos.y), WHITE);

    // Eye ball drawing
    // Calculate the eye's position based on the player's position
    Vector2 direction{8.0f, 0.0f};
    float magnitude = Remap(player_distance, 0.0f, distance_clamp, 0.0f, 1.0f);
    direction = Vector2Scale(direction, magnitude);
    direction = Vector2Rotate(direction, player_angle);
    // Direction will now hold the eye ball's center coordinate
    direction.x -= 12.0f;
    direction.y -= 12.0f;
    direction = Vector2Add(direction, {32.0f, 32.0f});

    EndTextureMode(); // Stop rendering to the canvas temporarily
    BeginTextureMode(eye_canvas);
    ClearBackground({0, 0, 0, 0});
    BeginShaderMode(mask_shader);
    SetShaderValueTexture(mask_shader, mask_loc, mask);
    DrawTexture(eye_ball, static_cast<int>(direction.x), static_cast<int>(direction.y), WHITE);
    EndShaderMode();
    EndTextureMode();
    BeginTextureMode(canvas); // Go back to rendering to the canvas again

    DrawTextureRec(eye_canvas.texture, {0, 0, static_cast<float>(WIDTH), static_cast<float>(-HEIGHT)}, pos, WHITE);
}

void LaserEye::update(GameScene *scene)
{
    if (is_dead)
    {
        frame_counter++;
        if (frame_counter >= death_time)
        {
            delete_entity = true;
            scene->nextLevel();
        }
        return;
    }

    switch (current_state)
    {
    case LaserEyeState::DELAY:
        frame_counter++;
        if (frame_counter % 2 == 0)
        {
            pos.x--; // Update the x position as it loads in
            if (pos.x <= (SPACE_CANVAS_WIDTH / 2.0f - WIDTH / 2.0f))
                pos.x = SPACE_CANVAS_WIDTH / 2.0f - WIDTH / 2.0f;
        }
        if (frame_counter >= delay_time)
        {
            current_state = LaserEyeState::SPAWNING;
            frame_counter = 0;
        }
        break;

    case LaserEyeState::SPAWNING:
        if (frames->completed)
            current_state = LaserEyeState::WAITING;
        break;

    case LaserEyeState::WAITING:
        frame_counter++;
        if (frame_counter >= wait_time)
            current_state = next_state;
        break;

    case LaserEyeState::ATTACK_1:
        attack1(scene);
        break;

    case LaserEyeState::ATTACK_2:
        attack2(scene);
        break;

    case LaserEyeState::ATTACK_3:
        attack3(scene);
        break;

    case LaserEyeState::ATTACK_4:
        attack4(scene);
        break;

    default:
        break;
    }

    // Update the eye's center at the end of the frame
    center = {pos.x + 32.0f, pos.y + 32.0f};

    // Track the player's position relative to the eye's center
    Vector2 player_pos = scene->player.getCenter();
    float dx = player_pos.x - center.x;
    float dy = player_pos.y - center.y;
    player_angle = atan2f(dy, dx);

    // Also get the player's distance from the eye's ceneter
    player_distance = Vector2Distance(player_pos, center);
}

void LaserEye::attack1(GameScene *scene)
{
    frame_counter++;

    if (frame_counter % attack_1_fire_time == 0)
    {
        // Move the bullet a bit outwards so it doesn't spawn on top of the shooter
        scene->spawnEntity(new EnemyProjectile(center, player_angle, 1.5f));
        scene->spawnEntity(new EnemyProjectile(center, player_angle + deg2rad(20), 1.5f));
        scene->spawnEntity(new EnemyProjectile(center, player_angle - deg2rad(20), 1.5f));
    }

    if (frame_counter >= attack_1_time)
    {
        frame_counter = 0;
        next_state = LaserEyeState::ATTACK_2;
        current_state = LaserEyeState::WAITING;
    }
}

void LaserEye::attack2(GameScene *scene)
{
    frame_counter++;

    if (frame_counter % attack_2_fire_time == 0)
    {
        scene->spawnEntity(new EnemyProjectile(center, 0, 1.5f));
        scene->spawnEntity(new EnemyProjectile(center, deg2rad(45), 1.5f));
        scene->spawnEntity(new EnemyProjectile(center, deg2rad(90), 1.5f));
        scene->spawnEntity(new EnemyProjectile(center, deg2rad(135), 1.5f));
        scene->spawnEntity(new EnemyProjectile(center, deg2rad(180), 1.5f));
        scene->spawnEntity(new EnemyProjectile(center, deg2rad(225), 1.5f));
        scene->spawnEntity(new EnemyProjectile(center, deg2rad(270), 1.5f));
        scene->spawnEntity(new EnemyProjectile(center, deg2rad(315), 1.5f));
    }

    if (frame_counter >= attack_2_time)
    {
        frame_counter = 0;
        next_state = LaserEyeState::ATTACK_3;
        current_state = LaserEyeState::WAITING;
    }
}

void LaserEye::attack3(GameScene *scene)
{
    frame_counter++;

    if (frame_counter % attack_3_fire_time == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            Vector2 bullet = {12.0f, 0.0f};
            float angle = attack_3_angle + 90.0f * static_cast<float>(i);
            bullet = Vector2Rotate(bullet, deg2rad(angle));
            bullet = Vector2Add(bullet, center);
            scene->spawnEntity(new SpiralBullet(bullet, center));
        }
        attack_3_angle += attack_3_angular_vel;
    }

    if (frame_counter >= attack_3_time)
    {
        frame_counter = 0;
        next_state = LaserEyeState::ATTACK_4;
        current_state = LaserEyeState::WAITING;
    }
}

void LaserEye::attack4(GameScene *scene)
{
    frame_counter++;

    if (frame_counter % attack_4_fire_time == 0)
    {
        float angle = static_cast<float>(GetRandomValue(-30, 30));
        // Move the bullet a bit outwards so it doesn't spawn on top of the shooter
        scene->spawnEntity(new EnemyProjectile(center, player_angle + deg2rad(angle), 1.25f));
    }

    if (frame_counter >= attack_4_time)
    {
        frame_counter = 0;
        next_state = LaserEyeState::ATTACK_1;
        current_state = LaserEyeState::WAITING;
    }
}