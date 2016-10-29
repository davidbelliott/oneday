#pragma once

class GameState;
class Terminal;

#include "EventSource.h"
#include <SFML/System.hpp>
#include <vector>

class Engine : public EventSource
{
public:
    Terminal* terminal;
    std::vector<GameState*> game_states;

	Engine();
	virtual ~Engine();

    void push_state(GameState* state);
    void pop_state();

    /* Inherited from EventSource */
    void push_event(Event* event);
    void handle_event(Event* event);
    void handle_events();

    /* Gets input events from the terminal and puts them in the top of the queue. */
    void get_input();
    void run(sf::Time dt);
    void draw();

    /* Event-creation functions */
    void disp(std::string s);

	bool paused;
	bool running;
};

