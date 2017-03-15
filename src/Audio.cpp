#include "Audio.h"

void Music::set_fade(Fade fade_in)
{
        fade = fade_in;
        if(fade == PLAY)
        {
            if(music.getStatus() != sf::Sound::Playing)
            {
                if(music.getStatus() == sf::Sound::Stopped)
                {
                    music.setVolume(0);
                    music.setPlayingOffset(initial_offset);
                    music.play();
                }
                else
                    music.play();
            }
            target_volume = 100;
            volume_step = 50;
        }
        else if(fade == PAUSE || fade == STOP)
        {
            target_volume = 0;
            volume_step = -50;
        }
}

void Music::update(sf::Time dt)
{
    if(fade != NONE)
    {
        if(cur_volume == target_volume)
        {
            if(fade == PAUSE)
                music.pause();
            else if(fade == STOP)
                music.stop();
            set_fade(NONE);
        }
        cur_volume = std::max(0.0, std::min(100.0, cur_volume + dt.asSeconds() * volume_step));
    }
    music.setVolume(cur_volume);
}

void Music::play()
{
    music.play();
}

void Music::pause()
{
    music.pause();
}

void Music::stop()
{
    music.stop();
}

Audio::Audio()
{
}

Audio::~Audio()
{
    for(auto it = musics.begin(); it != musics.end(); ++it)
        delete it->second;
}

void Audio::update(sf::Time dt)
{
    for(auto it = musics.begin(); it != musics.end(); ++it)
        it->second->update(dt);
}

void Audio::play_music(std::string music, sf::Time start_time)
{
    for(auto it = musics.begin(); it != musics.end(); ++it)
        it->second->set_fade(Music::PAUSE);
    if(musics.count(music) == 0)
    {
        musics[music] = new Music(music);
        musics[music]->set_fade(Music::PLAY);
        musics[music]->music.setPlayingOffset(start_time);
    }
    else
        musics[music]->set_fade(Music::PLAY);
}

void Audio::pause_music(std::string music)
{
    if(musics.count(music) > 0)
        musics[music]->set_fade(Music::PAUSE);
}

void Audio::stop_music(std::string music)
{
    if(musics.count(music) > 0)
        musics[music]->set_fade(Music::STOP);
}
