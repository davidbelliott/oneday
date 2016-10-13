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
	bool blocks_update;	//Does the state block the update of states beneath it?
	bool blocks_draw;	//Does the state block the draw of states beneath it?

	GameState(Engine* engine_in);
	virtual ~GameState();

	/*Called when the gamestate is placed in the engine.*/
	virtual void init();

	/*Called when the gamestate is removed from the engine.*/
	virtual void cleanup();

	/*Handles event from the queue.*/
	virtual void handle_event(Event* event);

	/*Update the gamestate for timestep dt.*/
	virtual void update(sf::Time dt);

	/*Draw the gamestate.*/
	virtual void draw(sf::RenderTarget* target);
};

