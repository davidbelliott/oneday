#include "Engine.h"
#include "GameState.h"
#include "common.h"
#include "Event.h"
#include "Console.h"
#include "Audio.h"
#include <iostream>

Engine::Engine()
:   running(true),
    console(new Console(this)),
    audio(new Audio()),
    window(new sf::RenderWindow(sf::VideoMode(config::window_width, config::window_height), "One Day in the Life of Young Jamal")),
    game_states()
{
}


Engine::~Engine()
{
    delete console;
    delete audio;
    delete window;
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

void Engine::get_input()
{
    console->get_input(window);
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
        game_states.back()->update(dt);
    }
    audio->update(dt);
}

void Engine::draw()
{
    if(!game_states.empty())
    {
        game_states.back()->draw(window);
    }
    window->display();
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
