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

// Audio Manager setup below
// =========================

static inline char *getMusicAssetFile(const char *file)
{
    const char *strings[] = {
        "assets/audio/music/",
        file};
    return TextJoin(const_cast<char **>(strings), 2, "");
}

AudioManager::AudioManager()
{
    music_list[0] = LoadMusicStream(getMusicAssetFile("start-screen.mp3"));
    music_list[1] = LoadMusicStream(getMusicAssetFile("credits-screen.mp3"));
    music_list[2] = LoadMusicStream(getMusicAssetFile("level-1.mp3"));
    music_list[3] = LoadMusicStream(getMusicAssetFile("level-1-boss.mp3"));
    music_list[4] = LoadMusicStream(getMusicAssetFile("level-2.mp3"));
    music_list[5] = LoadMusicStream(getMusicAssetFile("level-2-boss.mp3"));
    music_list[6] = LoadMusicStream(getMusicAssetFile("level-3.mp3"));
    music_list[7] = LoadMusicStream(getMusicAssetFile("level-3-boss.mp3"));
}

AudioManager::~AudioManager()
{
    for (size_t i = 0; i < MUSIC_FILES_NO; i++)
        UnloadMusicStream(music_list[i]);
}