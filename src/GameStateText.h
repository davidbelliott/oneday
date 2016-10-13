#pragma once
class World;
class Terminal;
class Parser;
class Config;
#include "GameState.h"
#include <string>

class GameStateText: public GameState
{
    public:
        World* world;
        Terminal* terminal;
        Parser* parser;
        std::string cur_user_string;

        GameStateText(Engine* engine_in, Config* config_in);
        virtual ~GameStateText();

        virtual void init();
        virtual void cleanup();
        virtual void handle_event(Event* event);
        virtual void update(sf::Time dt);
        virtual void draw(sf::RenderTarget* target);
};
