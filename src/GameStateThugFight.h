#pragma once
#include "GameState.h"
#include "Audio.h"

struct Fist
{
    double x, y;
    bool punching;
    sf::Time remaining_time;
    int color_index;
    bool dead;
};

struct Abs
{
    int health;
    int tense_ab;
};

struct Fragment
{
    double x, y;
    double vx, vy;
    sf::Time remaining_time;
    int color_index;
    bool dead;
};


class GameStateThugFight: public GameState
{
    public:
        // Resources
        std::shared_ptr<Music> music;

        // Entities
        Abs abs;
        std::vector<Fist> fists;
        std::vector<Fragment> fragments;
        
        // Sprites
        std::string thug_fist;
        std::string abs_str;
        std::string abs_tense_str;

        // Time counters
        sf::Time time_alive;
        sf::Time total_time;

        // Difficulty
        sf::Time time_since_spawn;
        int spawn_beats;
        int beats_to_wait;

        // Constants
        int ab_height;
        sf::Time beat;

        GameStateThugFight(Engine* engine_in);
        virtual ~GameStateThugFight();

        virtual void notify(event_ptr event);
        virtual void update(sf::Time dt);
        virtual void draw(sf::RenderTarget* target);
};
