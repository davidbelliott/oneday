#pragma once

#include "GameState.h"

class GameStateIntro: public GameState
{
    private:
        GameState* state_to_push;
    public:
        
        GameStateIntro(Engine* engine_in, GameState* state_to_push_in);
        virtual ~GameStateIntro();

        virtual void init();
        virtual void handle_event(Event* event);
        virtual void update(sf::Time dt);
};
