#pragma once

class Engine;
class Event;
#include "EventSink.h"
#include <SFML/Graphics.hpp>
#include <queue>

class GameState : public EventSink
{
public:
	Engine* engine;
	bool paused;		//Is this game state paused?
    bool running;       //Is this game state running?

	GameState(Engine* engine_in);
	virtual ~GameState();

	/*Called when the gamestate is placed in the engine.*/
	virtual void init();

	/*Called when the gamestate is removed from the engine.*/
	virtual void cleanup();

	/*If the gamestate is unpaused, pops events from the queue and passes each to handle_event.*/
	virtual void handle_events();

    /*Handles a single event.*/
    virtual void handle_event(Event* event);

	/*Run one cycle of the gamestate. dt is time elapsed in last engine cycle.*/
	virtual void run(sf::Time dt);

	/*Draw the gamestate.*/
	virtual void draw();
};

