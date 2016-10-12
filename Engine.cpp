#include "EventSource.h"
#include "Engine.h"
#include "Room.h"
#include "World.h"
#include "Terminal.h"
#include "common.h"

Engine::Engine(World* world_in, Terminal* terminal_in, Parser* parser_in)
: event_source(new EventSource()), world(world_in), terminal(terminal_in), parser(parser_in), paused(false), running(true)
{
}


Engine::~Engine()
{
    delete event_source;
}

void Engine::run()
{
	Action* action;
	world->set_current_room(world->get_current_room()->name.word, terminal);
	while (world->active)
	{
		action = parser->parse(terminal->get_input(), world, terminal);
		if(action)
			action->run(world, terminal);
	}
}
