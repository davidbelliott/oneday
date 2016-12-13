#pragma once

class Engine;
class World;
#include "Observer.h"
#include "Command.h"
#include <queue>

class GameState : public Observer
{
public:
	Engine* engine;
    World* world;
    std::queue<cmd_ptr> commands;
    std::queue<cmd_ptr> commands_priority;
    std::string last_command_result;
	bool paused;		//Is this game state paused?
    bool running;       //Is this game state running?

	GameState(Engine* engine_in);
	virtual ~GameState();

    /* Inherited from Observer. */
    virtual void notify(event_ptr event);

    /* Initializes the gamestate when it is pushed onto the engine. */
    virtual void init();

    /* Cleans up after the gamestate when it is removed from the engine. */
    virtual void cleanup();

    /* Sends a command to the gamestate's queue. */
    virtual void send(cmd_ptr command);

    /* Sends a command to the gamestate's priority queue
     * (takes precedence over all events in the normal queue). */
    virtual void send_front(cmd_ptr command);

    /* Executes a given command. */
    virtual void execute(cmd_ptr command);

    /* Executes the front command on the queue. Returns false if no more commands.*/
    virtual bool execute_front_command();

    /* Updates the gamestate with the given timestep. */
    virtual void update(sf::Time dt);

    /* Draws the gamestate. */
    virtual void draw();

    /* Pause the gamestate. */
    virtual void pause();

    /* Unpause the gamestate. */
    virtual void unpause();
};

