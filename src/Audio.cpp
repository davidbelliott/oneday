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
                    music.play();
                    music.setPlayingOffset(initial_offset);
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

Audio::Audio()
    : playing()
{
}

Audio::~Audio()
{
}

void Audio::update(sf::Time dt)
{
    for(int i = 0; i < playing.size(); )
    {
        playing[i]->update(dt);
        if(playing[i]->music.getStatus() != sf::Music::Playing)
            playing.erase(playing.begin() + i);
        else
            i++;
    }
}

void Audio::play_music(Music* music)
{
    music->set_fade(Music::PLAY);
    if(std::count(playing.begin(), playing.end(), music) == 0)
    {
        for(int i = 0; i < playing.size(); i++)
        {
            playing[i]->set_fade(Music::PAUSE);
        }
        playing.push_back(music);
    }
}
