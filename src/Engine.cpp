#include "Engine.h"
#include "GameState.h"
#include "Room.h"
#include "World.h"
#include "common.h"
#include "Event.h"
#include "level_data.cpp"
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
        event_ptr output_event = nullptr;
        if(sf_event.type == sf::Event::KeyPressed)
            output_event = std::make_shared<EventKeyPressed>(sf_event.key.code);
        else if(sf_event.type == sf::Event::TextEntered)
            output_event = std::make_shared<EventTextEntered>(static_cast<char>(sf_event.text.unicode));

        if(output_event)
            game_states.back()->notify(output_event);
    }
}

void Engine::run_commands()
{
    if(!game_states.empty())
    {
        game_states.back()->run_commands();
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

void Engine::run()
{
    sf::Clock clock;
    sf::Time dt;
    while(engine->running)
    {
        // Collect input from the user
        engine->get_input(window);

        // Let gamestates handle their pending events
        engine->handle_events();

        // Update gamestates based on elapsed time
        dt = clock.restart();
        update(dt);

        // Draw gamestates
        draw(window);
        window->display();

        // Sleep for remaining time
        while(clock.getElapsedTime().asSeconds() < 1.0f / config::update_frequency)
        {
            sf::sleep(sf::milliseconds(1.0f));
        }
    }
}
