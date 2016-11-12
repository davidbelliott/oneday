#pragma once

#include "GameState.h"

class GameStateIntro: public GameState
{
    private:
        //GameState* state_to_push;
        float x;
        float y;
        std::string title_string;
    public:
        
        GameStateIntro(Engine* engine_in);
        virtual ~GameStateIntro();

        virtual void notify(event_ptr event);
};
