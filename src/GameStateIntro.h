#pragma once

#include "GameState.h"
#include "Terminal.h"

class GameStateIntro: public GameState
{
    private:
        //GameState* state_to_push;
        Terminal* terminal;
    public:
        
        GameStateIntro(Engine* engine_in);
        virtual ~GameStateIntro();

        virtual void init();
        virtual void cleanup();
        virtual void handle_event(Event* event);
        virtual void update(sf::Time dt);
        virtual void draw(sf::RenderTarget* target);
};
