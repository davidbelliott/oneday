#pragma once

class World;
class GameState;

#include <SFML/Graphics.hpp>
#include <vector>

class Engine
{
public:
    World* world;
    std::vector<GameState*> game_states;

	Engine();
	virtual ~Engine();

    void push_state(GameState* state);
    void pop_state();

    /* Gets input events from the terminal and notifies gamestates. */
    void get_input(sf::Window* window);

    /* Tells gamestates to pop and run the commands on their queues. */
    void run_commands();

    void update(sf::Time dt);
    void draw(sf::RenderTarget* target);

    /* Event-creation functions */
    void disp(std::string s);

	bool running;
    bool ignore_next_event;
};

