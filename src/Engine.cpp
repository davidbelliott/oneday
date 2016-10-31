#include "Engine.h"
#include "GameState.h"
#include "Room.h"
#include "World.h"
#include "Terminal.h"
#include "common.h"
#include "Event.h"
#include <iostream>

Engine::Engine()
:   paused(false),
    running(true)
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

void Engine::get_input(sf::Window* window)
{
    sf::Event sf_event;
    while(window->pollEvent(sf_event))
    {
        Event* output_event = nullptr;
        if(sf_event.type == sf::Event::KeyPressed)
            output_event = new EventKeyPressed(sf_event.key.code);
        else if(sf_event.type == sf::Event::TextEntered)
            output_event = new EventTextEntered(static_cast<char>(sf_event.text.unicode));

        if(output_event)
            game_states.back()->add_event(output_event);
    }
}

void Engine::handle_events()
{
    if(!game_states.empty())
        game_states.back()->handle_events();
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

void Engine::draw(sf::RenderTarget* target)
{
    if(!game_states.empty())
        game_states.back()->draw(target);
}

void Engine::pause()
{
    if(!paused)
        paused = true;
}

void Engine::unpause()
{
    if(paused)
        paused = false;
}
