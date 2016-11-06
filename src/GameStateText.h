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
        Terminal* terminal;

        GameStateText(Engine* engine_in);
        virtual ~GameStateText();

        virtual void init();
        virtual void cleanup();
        virtual void handle_event(std::shared_ptr<Event> event);
        virtual void run(sf::Time dt);
        virtual void draw(sf::RenderTarget* target);
};
