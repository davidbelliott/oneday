#include "GameStateIntro.h"
#include "Event.h"
#include "Terminal.h"
#include "Engine.h"
#include "Config.h"
#include "CmdDisp.h"

GameStateIntro::GameStateIntro(Engine* engine_in)
    : GameState(engine_in)
{
}

GameStateIntro::~GameStateIntro()
{
}

void GameStateIntro::init()
{
    running = true;
    engine->register_sink(this, Event::KEY_PRESSED);
    Terminal* terminal = engine->terminal;
    //terminal->clr();
    for(int i = 0; i < config::N_COLORS; i++)
    {
        //terminal->set_color(config::colors[i]);
        engine->push_event(new CmdDisp("One Day in the Life of Young Jamal"));
    }
}

void GameStateIntro::cleanup()
{
    engine->unregister_sink(this, Event::KEY_PRESSED);
}

void GameStateIntro::notify(Event* event)
{
    if(event->type == Event::KEY_PRESSED)
    {
        running = false;
    }
}

void GameStateIntro::update(sf::Time dt)
{
}
