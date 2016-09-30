#pragma once

class GameEngine;
class Event;
#include "EventSink.h"
#include <SFML/Graphics.hpp>
#include <queue>

class GameState : public EventSink
{
protected:
	GameEngine* engine;
	bool paused;		//Is this game state paused?
	bool blocks_update;	//Does the state block the update of states beneath it?
	bool blocks_draw;	//Does the state block the draw of states beneath it?

public:
	GameState(GameEngine* engine_in);
	virtual ~GameState();

	virtual void set_engine(GameEngine* engine_in);
	virtual GameEngine* get_engine();
	virtual void set_paused(bool paused_in);
	virtual bool get_paused();
	virtual void set_blocks_update(bool blocks_update_in);
	virtual bool get_blocks_update();
	virtual void set_blocks_draw(bool blocks_draw_in);
	virtual bool get_blocks_draw();

	/*Called when the gamestate is placed in the engine.*/
	virtual void init();

	/*Called when the gamestate is removed from the engine.*/
	virtual void cleanup();

	/*Handles events from the queue.*/
	virtual void handle_events();

	/*Update the gamestate for timestep dt.*/
	virtual void update(sf::Time dt);

	/*Draw the gamestate.*/
	virtual void draw(sf::RenderTarget* target);
};

