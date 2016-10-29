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
        Parser* parser;
        std::string cur_user_string;

        GameStateText(Engine* engine_in);
        virtual ~GameStateText();

        virtual void init();
        virtual void cleanup();
        virtual void start_input();
        virtual void end_input();
        virtual void notify(Event* event);
        virtual void run(sf::Time dt);
        virtual void draw(sf::RenderTarget* target);
};
