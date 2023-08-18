#pragma once

#include <map>
#include <string>
#include <memory>
#include <fmod.hpp>
#include <vector>


namespace vortex
{
    // when playing, music audios are not supposed to overlap
    // while sound effects can overlap.
    // which affects the channel in audio player.
    enum class AudioType
    {
        AUDIO_TYPE_SFX,
        AUDIO_TYPE_MUSCI,
    };

    class AudioPlayer
    {
    private:
        std::shared_ptr<FMOD::System> m_System;
        std::shared_ptr<FMOD::Channel> m_MusicChannel;
        bool m_FadeIn;

        std::vector<std::shared_ptr<FMOD::Channel>> m_SfxChannels;
    public:
        AudioPlayer() {}
        bool IsPlayingMusic() { return true; }
        bool IsPlayingSfx() { return true; }
        void Play(const std::string& audio) {}
    };

    class AudioManager
    {
    private:
        std::shared_ptr<FMOD::System> m_System;
        AudioPlayer m_Player;
        std::map<std::string, std::shared_ptr<FMOD::Sound>> m_Audios;

    public:
        AudioManager();
        ~AudioManager();
        bool Load(const std::string& filePath, AudioType& type, bool stream = false);
        // bool Play(const std::string& filePath);
        // void ReleaseAudio();
        // void Update();

    };
}
