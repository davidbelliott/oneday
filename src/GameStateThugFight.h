#pragma once
#include "GameState.h"

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
    sf::Time remaining_time;
    double power;
    int health;
    bool tense;
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
        Abs abs;
        std::vector<Fist> fists;
        std::vector<Fragment> fragments;
        std::string thug_fist;
        std::string abs_str;
        std::string abs_tense_str;
        sf::Time ab_cooldown;
        sf::Time time_alive;
        sf::Time total_time;
        double spawn_countdown;

        GameStateThugFight(Engine* engine_in);
        virtual ~GameStateThugFight();

        virtual void notify(event_ptr event);
        virtual void update(sf::Time dt);
        virtual void draw(sf::RenderTarget* target);
};
