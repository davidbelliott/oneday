#pragma once

class GameState;
class Terminal;

#include "Audio.h"
#include "Event.h"
#include "Command.h"
#include "Observer.h"

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

    /* Gets input from the SFML window. */
    void get_input();

    /* Updates for the given time interval. */
    void update(unsigned int millis);

    /* Draws the current gamestate and displays the window. */
    void draw();

    /* Removes all non-running gamestates and quits if empty. */
    void prune();
};

