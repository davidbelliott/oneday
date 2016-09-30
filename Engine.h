#pragma once

class Terminal;
class World;

#include "Parser.h"

class Engine
{
public:
	World* world;
	Terminal* terminal;
	Parser* parser;

	Engine(World* world_in, Terminal* terminal_in, Parser* parser_in);
	virtual ~Engine();
	void run();

	bool paused;
	bool running;
};

