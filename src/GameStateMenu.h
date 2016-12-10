#pragma once
#include "GameState.h"

class GameStateMenu : public GameState
{
    public:
        std::string prompt;
        std::map<std::string, std::vector<cmd_ptr>> options;
        GameStateMenu(Engine* engine_in, std::string prompt_in,
                std::map<std::string, std::vector<cmd_ptr>> options_in);
        virtual ~GameStateMenu();

        virtual void init();
        virtual void cleanup();

        virtual void notify(event_ptr event);
};
