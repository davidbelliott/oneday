#pragma once
#include "GameState.h"
#include "Audio.h"

int round(double);

struct Fist
{
    double x, y;
    bool punching;
    sf::Time remaining_time;
    bool dead;
};

struct Abs
{
    int health;
    sf::Time tense[4];
};

struct Fragment
{
    double x, y;
    double vx, vy;
    sf::Time remaining_time;
    bool dead;
};

enum FistState
{
    MISSED = -2,
    BROKEN = -1,
    NONE = 0,
    UNBROKEN = 1
};


class GameStateThugFight: public GameState
{
    public:
        // Resources
        Music* music;

        sf::Time elapsed_time;
        int total_beats;
        double cur_beat;

        std::vector<FistState> beats[4];

        // Entities
        Abs abs;
        std::vector<Fragment> fragments;
        
        // Sprites
        std::string thug_fist;
        std::string abs_str;
        std::string abs_tense_str;

        // Constants
        int ab_height;
        sf::Time beat;

        GameStateThugFight(Engine* engine_in);
        virtual ~GameStateThugFight();

        int get_fist_x(int beat_offset);
        void load_beats();
        void break_fist(int index, int beat);
        void try_to_break(int index);
        int get_seconds_remaining();


        virtual void init();
        virtual void cleanup();
        virtual void notify(event_ptr event);
        virtual void update(sf::Time dt);
        virtual void draw();
        virtual void win();
        virtual void lose();
};
