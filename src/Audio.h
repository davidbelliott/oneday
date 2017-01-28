#pragma once
#include <SFML/Audio.hpp>
#include <memory>
#include <map>

class Music
{
    public:
    enum Fade
    {
        PLAY,
        PAUSE,
        STOP,
        NONE
    } fade;
    double cur_volume;
    double target_volume;
    double volume_step;
    sf::Time initial_offset;

    sf::Music music;

    Music(std::string filename_in, sf::Time initial_offset_in = sf::seconds(0.0))
        : music(),
        fade(NONE),
        cur_volume(0),
        target_volume(0),
        volume_step(0),
        initial_offset(initial_offset_in)
    {
        music.openFromFile(filename_in);
        music.setLoop(true);
        music.setPlayingOffset(initial_offset);
    }

    void update(sf::Time dt);
    void set_fade(Fade fade_in);
};

class Audio
{
    public:

        std::map<std::string, Music*> musics;

        Audio();
        virtual ~Audio();

        void update(sf::Time dt);
        void play_music(std::string music, sf::Time start_time = sf::seconds(0.0));
        void pause_music(std::string music);
        void stop_music(std::string music);
};
