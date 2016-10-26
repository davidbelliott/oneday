#include "GameStateIntro.h"
#include "Event.h"
#include "Terminal.h"
#include "Engine.h"
#include "Config.h"

GameStateIntro::GameStateIntro(Engine* engine_in, GameState* state_to_push_in)
    : GameState(engine_in), state_to_push(state_to_push_in)
{
}

GameStateIntro::~GameStateIntro()
{
}

void GameStateIntro::init()
{
    running = true;
    Terminal* terminal = engine->terminal;
    terminal->clr();
    for(int i = 0; i < config::N_COLORS; i++)
    {
        terminal->set_color(config::colors[i]);
        terminal->disp("One Day in the Life of Young Jamal");
    }
}

void GameStateIntro::handle_event(Event* event)
{
    if(event->type == Event::SFML)
    {
        sf::Event* sf_event = &event->sfml_event_data.sf_event;
        if(sf_event->type == sf::Event::KeyPressed && sf_event->key.code == sf::Keyboard::Space)
        {
            engine->push_state(state_to_push);
        }
    }
}

void GameStateIntro::update(sf::Time dt)
{
    

}
