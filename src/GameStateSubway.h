#pragma once
#include "GameState.h"

class GameStateSubway : public GameState
{
    private:

        enum Terrain
        {
            NORMAL = 0,
            NO_FLOOR,
            LOW_CEILING
        };

        int total_beats;
        double cur_beat;
        sf::Time elapsed_time;

        std::vector<Terrain> terrain;
        float gravity;

        struct Player
        {
            enum State
            {
                NORMAL,
                JUMP,
                DUCK,
                FALL
            } state;
            float y;
            float vy;
            std::string sprite;
            std::string sprite_1;
            int n_draws;
        } player;

        sf::Time beat;
        Music* music;

    public:

        GameStateSubway(Engine* engine_in);
        virtual ~GameStateSubway();

        int get_seconds_remaining();
        int get_x(int beat);

        virtual void load_beats();
        virtual void init();
        virtual void cleanup();
        virtual void notify(event_ptr event);
        virtual void update(sf::Time dt);
        virtual void draw();
        virtual void jump();
        virtual void duck();
        virtual void win();
        virtual void lose();
};
