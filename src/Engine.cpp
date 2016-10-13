#include "EventSource.h"
#include "Engine.h"
#include "GameState.h"
#include "Room.h"
#include "World.h"
#include "Terminal.h"
#include "common.h"

Engine::Engine(sf::RenderWindow* window_in)
: paused(false), running(true), window(window_in)
{
}


Engine::~Engine()
{
}

void Engine::draw()
{
    window->clear();
    game_states.back()->draw(window);
    window->display();
}

void Engine::get_input()
{
    sf::Event sf_event;
    Event event;
    event.type = Event::SFML;

    while(window->pollEvent(sf_event))
    {
        if(sf_event.type == sf::Event::KeyPressed && sf_event.key.code == sf::Keyboard::Escape)
        {
            running = false;
        }
        else
        {
            event.sfml_event_data.sf_event = sf_event;
            game_states.back()->handle_event(&event);
        }
    }
}


void Engine::update(sf::Time dt)
{
    game_states.back()->update(dt);
}
