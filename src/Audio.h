#pragma once
#include <SFML/Audio.hpp>
#include <memory>

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

    Music(std::string filename_in)
        : music(),
        fade(NONE),
        cur_volume(0),
        target_volume(0),
        volume_step(0),
        initial_offset(sf::seconds(0.0))
    {
        music.openFromFile(filename_in);
        music.setLoop(true);
    }

    void update(sf::Time dt);

    void set_fade(Fade fade_in);
};

class Audio
{
    public:

        std::vector<Music*> playing;

        Audio();
        virtual ~Audio();

        void update(sf::Time dt);
        void play_music(Music* music);
};
