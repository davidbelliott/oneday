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
    bool running;       //Is this game state running?

	GameState(Engine* engine_in);
	virtual ~GameState();

    /* Initializes the gamestate when it is pushed onto the engine. */
    virtual void init();

    /* Cleans up after the gamestate when it is removed from the engine. */
    virtual void cleanup();

    /* Inherited from Observer. */
    virtual void notify(event_ptr event);

    /* Updates the gamestate with the given timestep. */
    virtual void update(sf::Time dt);

    /* Draws the gamestate. */
    virtual void draw();
};

