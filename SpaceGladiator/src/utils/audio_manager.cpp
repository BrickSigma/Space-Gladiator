#include "utils/audio_manager.hpp"

#include <format>
#include <string>

AudioManager &AudioManager::getInstance()
{
    static AudioManager instance;
    return instance;
}

void AudioManager::update() const
{
    if (current_track == nullptr)
        return;

    UpdateMusicStream(*current_track);
}

void AudioManager::startMusicFile(MusicFile file)
{
    size_t file_index = static_cast<size_t>(file);

    // Stop the current track if there's one playing
    if (current_track != nullptr)
        StopMusicStream(*current_track);

    current_track = &music_list[file_index];
    PlayMusicStream(*current_track);
}

void AudioManager::stopMusic()
{
    if (current_track == nullptr)
        return;

    StopMusicStream(*current_track);
    current_track = nullptr;
}

void AudioManager::pauseMusic() const
{
    if (current_track != nullptr)
        PauseMusicStream(*current_track);
}

void AudioManager::resumeMusic() const
{
    if (current_track != nullptr)
        ResumeMusicStream(*current_track);
}

void AudioManager::playSFX(SFXFile file) const
{
    PlaySound(sfxs[static_cast<size_t>(file)]);
}

// Audio Manager setup below
// =========================

static inline char *getMusicAssetFile(const char *file)
{
    const char *strings[] = {
        "assets/audio/music/",
        file};
    return TextJoin(const_cast<char **>(strings), 2, "");
}

static inline char *getSFXAssetFile(const char *file)
{
    const char *strings[] = {
        "assets/audio/sfx/",
        file};
    return TextJoin(const_cast<char **>(strings), 2, "");
}

AudioManager::AudioManager()
{
    // Load the background music files
    music_list[0] = LoadMusicStream(getMusicAssetFile("start-screen.mp3"));
    music_list[1] = LoadMusicStream(getMusicAssetFile("credits-screen.mp3"));
    music_list[2] = LoadMusicStream(getMusicAssetFile("level-1.mp3"));
    music_list[3] = LoadMusicStream(getMusicAssetFile("level-1-boss.mp3"));
    music_list[4] = LoadMusicStream(getMusicAssetFile("level-2.mp3"));
    music_list[5] = LoadMusicStream(getMusicAssetFile("level-2-boss.mp3"));
    music_list[6] = LoadMusicStream(getMusicAssetFile("level-3.mp3"));
    music_list[7] = LoadMusicStream(getMusicAssetFile("level-3-boss.mp3"));

    // Load the sound effects
    sfxs[0] = LoadSound(getSFXAssetFile("boss-explosion.wav"));
    sfxs[1] = LoadSound(getSFXAssetFile("enemy-explosion.wav"));
    sfxs[2] = LoadSound(getSFXAssetFile("game-over.mp3"));
    sfxs[3] = LoadSound(getSFXAssetFile("laser.wav"));
    sfxs[4] = LoadSound(getSFXAssetFile("player-explosion.wav"));
    sfxs[5] = LoadSound(getSFXAssetFile("powerup.wav"));
    sfxs[6] = LoadSound(getSFXAssetFile("shoot.wav"));
    sfxs[7] = LoadSound(getSFXAssetFile("start.mp3"));
}

AudioManager::~AudioManager()
{
    for (size_t i = 0; i < MUSIC_FILES_NO; i++)
        UnloadMusicStream(music_list[i]);

    for (size_t i = 0; i < SFX_FILES_NO; i++)
        UnloadSound(sfxs[i]);
}