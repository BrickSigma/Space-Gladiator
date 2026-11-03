#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "scene_manager.hpp"
#include "entities/entity.hpp"
#include "entities/player.hpp"
#include "constants.hpp"
#include "levels/levels.hpp"

#include <vector>
#include <raylib.h>

#define SCORE_BOARD_HEIGHT (24)
#define SPACE_CANVAS_WIDTH (CANVAS_WIDTH)
#define SPACE_CANVAS_HEIGHT (CANVAS_HEIGHT - SCORE_BOARD_HEIGHT)

class GameScene : public Scene
{
public:
    GameScene(RenderTexture *screen);
    ~GameScene();

    // Used to restart the game state from a fresh
    void reset();

    // Used to restart from a level, without resetting the score
    void resetFromCheckpoint();

    // Called in the update loop
    SceneState update() override;

    // Handle entering the game scene from another state
    void onEnter(SceneState previous_state) override;

    // Used to spawn a new entity
    void spawnEntity(Entity *entity);

    // Current distance into the level
    unsigned long scroll_distance = 0;

    // Current score
    unsigned long score = 0;

    // Internal level manager
    LevelManager level_manager{};

    // Pointer to player object for quick access by other entities
    Player *player = nullptr;

    // List of all entities in the system;
    std::vector<Entity *> entities{};

private:
    // Queue for any entities to add at the end of the game loop
    std::vector<Entity *> new_entities{};

    // Render texture used for showing all the entities and separating the view from the score board
    RenderTexture canvas;

    // Internal frame counter used to adjust the distance
    int frame_counter = 0;
    const int DISTANCE_UPDATE_DURATION = 2;  // Advance the scroll at 1 pixel every 2 frames (30 pixels/second)

    bool is_paused = false;
};

#endif // GAME_SCENE_HPP