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
        std::vector<Fist> fists;
        std::vector<Fragment> fragments;
        std::string thug_fist;
        std::string your_fist;

        GameStateThugFight();
        virtual ~GameStateThugFight();

        virtual void init();
        virtual void cleanup();
        virtual void handle_event(Event* event);
        virtual void run(sf::Time dt);
        virtual void draw(sf::RenderTarget* target);
};
