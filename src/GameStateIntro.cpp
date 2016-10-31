#include "GameStateIntro.h"
#include "Event.h"
#include "Engine.h"
#include "Config.h"

GameStateIntro::GameStateIntro(Engine* engine_in)
    : GameState(engine_in),
    terminal(new Terminal())
{
}

GameStateIntro::~GameStateIntro()
{
}

void GameStateIntro::init()
{
    running = true;
    terminal->clr();
    for(int i = 0; i < config::N_COLORS; i++)
    {
        terminal->set_color(config::colors[i]);
        terminal->disp("One Day in the Life of Young Jamal");
    }
    terminal->input_mode();
}

void GameStateIntro::cleanup()
{
}

void GameStateIntro::handle_event(Event* event)
{
    /*if(event->type == Event::KEY_PRESSED)
    {
        running = false;
    }*/
    terminal->handle_event(event);
}

void GameStateIntro::update(sf::Time dt)
{
}

void GameStateIntro::draw(sf::RenderTarget* target)
{
    terminal->draw(target);
}
