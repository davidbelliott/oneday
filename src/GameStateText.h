#pragma once
#include "Parser.h"
#include "GameState.h"

class GameStateText: public GameState
{
    public:
        Parser* parser;
        std::string line;

        GameStateText(Engine* engine_in);
        virtual ~GameStateText();

        virtual void init();
        virtual void cleanup();

        virtual void notify(event_ptr event);
        virtual void update(unsigned int millis);
};
