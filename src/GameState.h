#pragma once

class Engine;
#include "Receiver.h"
#include <SFML/Graphics.hpp>
#include <queue>

class GameState : public Receiver
{
public:
	Engine* engine;
	bool paused;		//Is this game state paused?
    bool running;       //Is this game state running?

	GameState(Engine* engine_in);
	virtual ~GameState();

    virtual void add_event(std::shared_ptr<Event> event);

	/*Called when the gamestate is placed in the engine.*/
	virtual void init();

	/*Called when the gamestate is removed from the engine.*/
	virtual void cleanup();

    /* Handles an event from the mailbox. */
    virtual void handle_event(std::shared_ptr<Event> event);

    /* Handles all events from the mailbox by popping from the queue and calling handle_event on each. */
    virtual void handle_events();

	/*Run one cycle of the gamestate. dt is time elapsed in last engine cycle.*/
	virtual void run(sf::Time dt);

	/*Draw the gamestate.*/
	virtual void draw(sf::RenderTarget* target);
};

