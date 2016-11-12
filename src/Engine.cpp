#include "Engine.h"
#include "GameState.h"
#include "Room.h"
#include "World.h"
#include "common.h"
#include "Event.h"
#include "level_data.h"
#include <iostream>

Engine::Engine()
:   running(true),
    world(generate_world())
{
}


Engine::~Engine()
{
    delete world;
}

void Engine::push_state(GameState* state)
{
    game_states.push_back(state);
}

void Engine::pop_state()
{
    if(!game_states.empty())
    {
        game_states.pop_back();
    }
}

void Engine::notify_gamestates(event_ptr event)
{
    if(!game_states.empty())
    {
        game_states.back()->notify(event);
    }
}

void Engine::command_gamestates(cmd_ptr command)
{
    if(!game_states.empty())
    {
        game_states.back()->send(command);
    }
}

void Engine::execute_commands()
{
    if(!game_states.empty())
    {
        game_states.back()->execute_commands();
    }
}

void Engine::update(sf::Time dt)
{
    if(!game_states.empty())
    {
        event_ptr update_event = std::make_shared<EventUpdate>(dt);
        game_states.back()->notify(update_event);
    }
}

void Engine::draw(sf::RenderTarget* target)
{
    if(!game_states.empty())
    {
        event_ptr draw_event = std::make_shared<EventDraw>(target);
        game_states.back()->notify(draw_event);
    }
}

void Engine::prune()
{
    while(!game_states.empty() && !game_states.back()->running)
    {
        game_states.pop_back();
    }
    if(game_states.empty())
        running = false;
}
