#include "Engine.h"
#include "GameState.h"
#include "Room.h"
#include "World.h"
#include "Terminal.h"
#include "common.h"

Engine::Engine(Terminal* terminal_in)
:   EventSource(),
    paused(false),
    running(true),
    terminal(terminal_in)
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

void Engine::push_event(Event* event)
{
    if(paused && event->type == Event::SFML && event->sfml_event_data.sf_event.type == sf::Event::KeyPressed)
        paused = false;

    EventSource::push_event(event);
}

void Engine::handle_event(Event* event)
{
    if(!paused && event->type == Event::CMD_PAUSE)
        paused = true;
}

void Engine::handle_events()
{
    while(!paused && !incoming_queue.empty())
    {
        Event* front_event = incoming_queue.front();
        incoming_queue.pop();
        handle_event(front_event);
        send_event(front_event);
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
    if(!paused && !game_states.empty())
        game_states.back()->run(dt);
    while(game_states.size() > 0 && !game_states.back()->running)
        pop_state();
    if(game_states.size() == 0)
        running = false;
}
