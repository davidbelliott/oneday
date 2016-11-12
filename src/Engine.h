#pragma once

class World;
class GameState;

#include "Event.h"
#include "Command.h"

#include <SFML/Graphics.hpp>
#include <vector>

class Engine
{
public:
    World* world;
    std::vector<GameState*> game_states;
	bool running;

	Engine();
	virtual ~Engine();

    void push_state(GameState* state);
    void pop_state();

    void notify_gamestates(event_ptr event);
    void command_gamestates(cmd_ptr command);

    /* Tells gamestates to pop and run the commands on their queues. */
    void execute_commands();

    void update(sf::Time dt);
    void draw(sf::RenderTarget* target);

    /* Remove all non-running gamestates and quit if empty. */
    void prune();
};

