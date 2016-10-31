#include "GameStateText.h"
#include "Action.h"
#include "Event.h"
#include "Parser.h"
#include "Room.h"
#include "Terminal.h"
#include "World.h"
#include "Config.h"
#include "level_data.h"
#include "Engine.h"

GameStateText::GameStateText(Engine* engine_in)
    : GameState(engine_in),
    terminal(new Terminal())
{
    terminal->owner = this;
}

GameStateText::~GameStateText()
{
    delete terminal;
}

void GameStateText::init()
{
    running = true;
    world = generate_world();
    parser = new Parser();
	world->set_current_room(world->get_current_room()->name.word, this);
}

void GameStateText::cleanup()
{
    delete world;
    delete parser;
    terminal->clr();
}

void GameStateText::handle_event(Event* event)
{
    if(event->type == Event::USER_LINE)
    {
        std::string line = static_cast<EventUserLine*>(event)->line;
        if(line == "")
            terminal->disp("Please enter a command.");
        else
        {
            Action* action = parser->parse(line, world, this);
            if(action)
            {
                action->run(world, this);
            }
            else
            {
                terminal->disp("I don't understand.");
            }
            if(!world->active)
                running = false;
        }
        terminal->input_mode();
    }
    terminal->handle_event(event);
}

void GameStateText::run(sf::Time dt)
{
}

void GameStateText::draw(sf::RenderTarget* target)
{
    terminal->draw(target);
}
