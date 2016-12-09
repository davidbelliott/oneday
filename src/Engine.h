#pragma once

class World;
class GameState;
class Console;
class Audio;

#include "Event.h"
#include "Command.h"

#include <SFML/Graphics.hpp>
#include <vector>

class Engine
{
public:
    World* world;
    Console* console;
    Audio* audio;
    sf::RenderWindow* window;
    std::vector<GameState*> game_states;
	bool running;

	Engine();
	virtual ~Engine();

    void push_state(GameState* state);
    void pop_state();

    void notify_gamestates(event_ptr event);
    void command_gamestates(cmd_ptr command);

    /* Gets input from the SFML window. */
    void get_input();

    /* Tells gamestates to pop and run the commands on their queues. */
    void execute_commands();

    /* Updates for the given time interval. */
    void update(sf::Time dt);

    /* Draws the current gamestate and displays the window. */
    void draw();

    /* Removes all non-running gamestates and quits if empty. */
    void prune();
};

