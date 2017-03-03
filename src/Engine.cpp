#include "Engine.h"
#include "GameState.h"
#include "Terminal.h"
#include "common.h"
#include "Event.h"
#include <iostream>

Engine::Engine()
:   running(true),
    terminal(new Terminal()),
    game_states()
{
}


Engine::~Engine()
{
    delete terminal;
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
        GameState* remove = game_states.back();
        game_states.pop_back();
        remove->cleanup();
        delete remove;
        remove = nullptr;
    }
}

void Engine::notify(event_ptr event)
{
    if(!game_states.empty())
    {
        game_states.back()->notify(event);
    }
}

void Engine::get_input()
{
    //terminal->get_input();
}

void Engine::update(unsigned int millis)
{
    if(!game_states.empty())
    {
        game_states.back()->update(millis);
    }
}

void Engine::draw()
{
    if(!game_states.empty())
    {
        game_states.back()->draw();
    }
    terminal->refresh_display();
}

void Engine::prune()
{
    for(int i = 0; i < game_states.size(); )
    {
        if(!game_states[i]->running)
        {
            game_states[i]->cleanup();
            delete game_states[i];
            game_states.erase(game_states.begin() + i);
        }
        else
            i++;
    }
    if(game_states.empty())
        running = false;
}
