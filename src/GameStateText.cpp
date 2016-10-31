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
    : GameState(engine_in)
{
}

GameStateText::~GameStateText()
{
}

void GameStateText::init()
{
    running = true;
    world = generate_world();
    parser = new Parser();
    Action* change_room_action = parser->action_factory.create_action(parser->word_list.get_word("look"));
    world->change_room_action = change_room_action;
	world->set_current_room(world->get_current_room()->name.word, engine);
    engine->push_event(new CmdInput());
}

void GameStateText::cleanup()
{
    engine->unregister_sink(this, Event::USER_LINE);
    engine->unregister_sink(this, Event::CMD_SET_OBJECTIVE);
    delete world;
    delete parser;
    engine->terminal->clr();
}

void GameStateText::notify(Event* event)
{
    if(event->type == Event::USER_LINE)
    {
        Action* action = parser->parse(static_cast<EventUserLine*>(event)->line, world, engine);
        if(action)
            action->run(world, engine);
        if(!world->active)
            running = false;
        engine->push_event(new CmdInput());
    }
    else if(event->type == Event::CMD_SET_OBJECTIVE)
    {
        engine->push_event(new CmdSetColor(sf::Color::Red));
        engine->push_event(new CmdDisp("New objective: " + static_cast<CmdSetObjective*>(event)->objective + "."));
    }
}

void GameStateText::run(sf::Time dt)
{
}

void GameStateText::draw(sf::RenderTarget* target)
{
}
