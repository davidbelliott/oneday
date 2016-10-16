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
    : GameState(engine_in), cur_user_string("")
{
    world = generate_world();
    parser = new Parser();
    Action* change_room_action = parser->action_factory.create_action(parser->word_list.get_word("look"));
    world->change_room_action = change_room_action;
	world->set_current_room(world->get_current_room()->name.word, engine->terminal);
}

GameStateText::~GameStateText()
{
    delete world;
    delete parser;
}

void GameStateText::init()
{
}

void GameStateText::cleanup()
{
}

void GameStateText::handle_event(Event* event)
{
}

void GameStateText::run(sf::Time dt)
{
    std::string user_input = engine->terminal->get_input();
    Action* action = parser->parse(user_input, world, engine->terminal);
    if(action)
        action->run(world, engine->terminal);
    if(!world->active)
        running = false;
}

void GameStateText::draw(sf::RenderTarget* target)
{
}
