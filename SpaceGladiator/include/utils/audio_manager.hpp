#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <raylib.h>
#include <cstddef>

enum class MusicFile
{
    START_SCREEN,
    CREDITS_SCREEN,
    LEVEL_1,
    LEVEL_1_BOSS,
    LEVEL_2,
    LEVEL_2_BOSS,
    LEVEL_3,
    LEVEL_3_BOSS
};

enum class SFXFile {
    BOSS_EXPLOSION,
    ENEMEY_EXPLOSION,
    GAME_OVER,
    LASER,
    PLAYER_EXPLOSION,
    POWERUP,
    SHOOT,
    START
};

/**
 * Audio manager class.
 *
 * (Yes, I know it's a singleton and there is probably a better way to handle
 * audio output, but for simplicity sake this'll work for now, so deal with it.)
 */
class AudioManager
{
private:
    AudioManager(const AudioManager &) = delete;
    AudioManager &operator=(const AudioManager &) = delete;

    AudioManager();
    ~AudioManager();

    // Tracks the current music stream being played (only 1 is allowed to play at a time)
    Music *current_track{nullptr};

public:
    // Get an instance of the singleton
    static AudioManager &getInstance();

    // Update the music stream (must be called in the main update loop)
    void update() const;

    // Start playing a music file from the beginning
    void startMusicFile(MusicFile file);

    // Stop the music from playing (used when the player dies)
    void stopMusic();

    // Pause the music
    void pauseMusic() const;

    // Resume the music
    void resumeMusic() const;
    
    // Play a sound effect
    void playSFX(SFXFile file) const;

    static constexpr size_t MUSIC_FILES_NO{8};
    static constexpr size_t SFX_FILES_NO{8};

    // Array of all the music files loaded into the audio manager
    Music music_list[MUSIC_FILES_NO]{};

    Sound sfxs[SFX_FILES_NO]{};
};

#endif // AUDIO_MANAGER_HPP