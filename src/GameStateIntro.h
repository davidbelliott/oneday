#pragma once

#include "GameState.h"

class GameStateIntro: public GameState
{
    private:
        //GameState* state_to_push;
    public:
        
        GameStateIntro(Engine* engine_in);
        virtual ~GameStateIntro();

        virtual void init();
        virtual void cleanup();
        virtual void notify(Event* event);
        virtual void update(sf::Time dt);
};
