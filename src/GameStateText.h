#pragma once
#include "Parser.h"
#include "GameState.h"

class GameStateText: public GameState
{
    public:
        Parser* parser;

        GameStateText(Engine* engine_in);
        virtual ~GameStateText();

        virtual void notify(event_ptr event);
};
