#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "scenes/scene_manager.hpp"
#include "entities/entity.hpp"
#include "entities/player/player.hpp"
#include "utils/constants.hpp"
#include "level_manager/level_manager.hpp"
#include "utils/camera_shake.hpp"

#include <vector>
#include <raylib.h>

#define SCORE_BOARD_HEIGHT (16)
#define SPACE_CANVAS_WIDTH (CANVAS_WIDTH)
#define SPACE_CANVAS_HEIGHT (CANVAS_HEIGHT - SCORE_BOARD_HEIGHT)

#define PLAYER_DEATH_TIME (150)
#define GAME_OVER_TIME (300)

enum class GameSceneState : int
{
    PAUSED,        // The game is paused
    LEVEL,         // Currently in a level
    PLAYER_KILLED, // The player has been killed
    TRANSITION,    // Moving between levels
    COMPLETED,     // The last level was completed
    GAME_OVER      // Used to indicate that the player lost all lives and we should return to the title screen
};

class GameScene : public Scene
{
public:
    GameScene(RenderTexture *screen_);
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

    // Push a new game scene state to the event queue for processing
    void pushStateEvent(GameSceneState new_state);

    // Change the camera shake instance.
    void setCameraShake(CameraShake camera);

    // Current distance into the level
    int scroll_distance{0};

    // Current score
    unsigned long score{0};

    // Internal level manager
    LevelManager level_manager{};

    // Pointer to player object for quick access by other entities
    Player player{};

    // List of all entities in the system;
    std::vector<Entity *> entities{};

private:
    // Queue for any entities to add at the end of the game loop
    std::vector<Entity *> new_entities{};

    // Render texture used for showing all the entities and separating the view from the score board
    RenderTexture canvas;

    // Internal frame counter used to adjust the distance
    int frame_counter{0};
    const int DISTANCE_UPDATE_DURATION{2}; // Advance the scroll at 1 pixel every 2 frames (30 pixels/second)

    bool stop_scrolling{false};

    // The current state of the game scene
    GameSceneState state{GameSceneState::LEVEL};

    // Event queue for the state transitions
    std::vector<GameSceneState> state_event_queue{};

    const int transition_duration{180};          // Make the transition last ~3 seconds
    int player_killed_timer{PLAYER_DEATH_TIME}; // Player death timer (~2.5 seconds or ~5 seconds on game over)

    // Camera shake object
    CameraShake camera{0, 0};

    // Used to update all the entities
    void updateEntities();

    /**
     * Handle the state transitions here.
     *
     * This also looks at an internal state queue for entities
     * and should resolve two entities trying to update the state simultaneously.
     */
    void handleStateChange();
};

#endif // GAME_SCENE_HPP