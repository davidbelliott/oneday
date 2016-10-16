#pragma once

class GameState;
class EventSource;
class Terminal;
class World;

#include "Parser.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Engine
{
public:
    Terminal* terminal;
    std::vector<GameState*> game_states;

	Engine(Terminal* terminal_in);
	virtual ~Engine();

    void push_state(GameState* state);
    void pop_state();

    void get_input();
    void run(sf::Time dt);
    void draw();

	bool paused;
	bool running;
};

