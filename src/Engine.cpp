#include "EventSource.h"
#include "Engine.h"
#include "GameState.h"
#include "Room.h"
#include "World.h"
#include "Terminal.h"
#include "common.h"

Engine::Engine(Terminal* terminal_in)
: paused(false), running(true), terminal(terminal_in), event_source(new EventSource())
{
}


Engine::~Engine()
{
}

void Engine::push_state(GameState* state)
{
    game_states.push_back(state);
    state->init();
}

void Engine::pop_state()
{
    if(!game_states.empty())
    {
        game_states.back()->cleanup();
        game_states.pop_back();
    }
}

void Engine::get_input()
{
    sf::Event terminal_event;
    while(terminal->get_event(&terminal_event))
    {
        Event event;
        event.type = Event::SFML;
        event.sfml_event_data.sf_event = terminal_event;
        if(!game_states.empty())
            game_states.back()->handle_event(&event);
    }

    
}

void Engine::draw()
{
    if(!game_states.empty())
        game_states.back()->draw();
    terminal->draw();
}

void Engine::run(sf::Time dt)
{
    if(!game_states.empty())
        game_states.back()->run(dt);
    while(game_states.size() > 0 && !game_states.back()->running)
        pop_state();
    if(game_states.size() == 0)
        running = false;
}
