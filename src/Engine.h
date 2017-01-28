#pragma once

class GameState;
class Terminal;

#include "Event.h"
#include "Command.h"
#include "Observer.h"
#include "Audio.h"

#include <SFML/Graphics.hpp>
#include <vector>

class Engine : public Observer
{
public:
    Terminal* terminal;
    Audio* audio;
    std::vector<GameState*> game_states;
	bool running;

	Engine();
	virtual ~Engine();

    void push_state(GameState* state);
    void pop_state();

    void notify(event_ptr event);
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

