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
    sf::RenderWindow* window;
    std::vector<GameState*> game_states;

	Engine(sf::RenderWindow* window_in);
	virtual ~Engine();

    void get_input();
    void update(sf::Time dt);
    void draw();

	bool paused;
	bool running;
};

