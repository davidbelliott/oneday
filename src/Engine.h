#pragma once

class GameState;
class Terminal;
class World;

#include "EventSource.h"
#include "Parser.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Engine : public EventSource
{
public:
    Terminal* terminal;
    std::vector<GameState*> game_states;

	Engine(Terminal* terminal_in);
	virtual ~Engine();

    void push_state(GameState* state);
    void pop_state();

    void handle_events();
    void run(sf::Time dt);
    void draw();

	bool paused;
	bool running;
};

