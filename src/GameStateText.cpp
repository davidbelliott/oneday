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
    add_event(std::make_shared<CmdInput>());
}

void GameStateText::cleanup()
{
    delete world;
    delete parser;
    terminal->clr();
}

void GameStateText::handle_event(std::shared_ptr<Event> event)
{
    if(event->type == Event::USER_LINE)
    {
        std::string line = std::static_pointer_cast<EventUserLine>(event)->line;
        if(line == "")
            add_event(std::make_shared<CmdDisp>("Please enter a command."));
        else
        {
            Action* action = parser->parse(line, world, this);
            if(action)
            {
                action->run(world, this);
            }
            else
            {
                add_event(std::make_shared<CmdDisp>("I don't understand."));
            }
            if(!world->active)
                running = false;
        }
        add_event(std::make_shared<CmdInput>());
    }
    else if(event->type == Event::CMD_ADD_GAMESTATE)
    {
        GameState* state_to_add = std::static_pointer_cast<CmdAddGameState>(event)->state_to_add;
        engine->push_state(state_to_add);
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
