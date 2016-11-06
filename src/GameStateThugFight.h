#pragma once
class Terminal;
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
        Terminal* terminal;
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

        GameStateThugFight();
        virtual ~GameStateThugFight();

        virtual void init();
        virtual void cleanup();
        virtual void handle_event(std::shared_ptr<Event> event);
        virtual void run(sf::Time dt);
        virtual void draw(sf::RenderTarget* target);
};
