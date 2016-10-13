#include "Config.h"
#include "Engine.h"
#include "Parser.h"
#include "World.h"
#include "Terminal.h"
#include "level_data.h"
#include <iostream>

int main()
{
	Config* config = new Config();
	Terminal* terminal = new Terminal(config);
	Parser* parser = new Parser();
	World* world = generate_world();
	{
		Action* change_room_action = parser->action_factory.create_action(parser->word_list.get_word("look"));
		world->change_room_action = change_room_action;
	}


	

	Engine* engine = new Engine(world, terminal, parser);
	engine->run();

	delete config;
	delete world;
	delete terminal;
	delete parser;
	delete engine;

	return 0;
}
