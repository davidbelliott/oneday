#include "Engine.h"
#include "GameState.h"
#include "World.h"
#include "common.h"
#include "Event.h"
#include "Console.h"
#include <iostream>

Engine::Engine()
:   running(true),
    world(new World()),
    console(new Console(this)),
    window(new sf::RenderWindow(sf::VideoMode(config::window_width, config::window_height), "One Day in the Life of Young Jamal")),
    game_states()
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
    while(!game_states.empty() && !game_states.back()->running)
    {
        game_states.pop_back();
    }
    if(game_states.empty())
        running = false;
}
