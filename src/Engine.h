#pragma once

class GameState;

#include <SFML/Graphics.hpp>
#include <vector>

class Engine
{
public:
    std::vector<GameState*> game_states;

	Engine();
	virtual ~Engine();

    void push_state(GameState* state);
    void pop_state();

    /* Gets input events from the terminal and puts them in the top of the queue. */
    void get_input(sf::Window* window);
    void handle_events();
    void run(sf::Time dt);
    void draw(sf::RenderTarget* target);

    void pause();
    void unpause();

    /* Event-creation functions */
    void disp(std::string s);

	bool paused;
	bool running;
    bool ignore_next_event;
};

