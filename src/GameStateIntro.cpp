#include "GameStateIntro.h"
#include "Event.h"
#include "Engine.h"
#include "Config.h"

GameStateIntro::GameStateIntro(Engine* engine_in)
    : GameState(engine_in),
    terminal(new Terminal()),
    title_string("ONE DAY IN THE LIFE OF YOUNG JAMAL")
{
}

GameStateIntro::~GameStateIntro()
{
}

void GameStateIntro::init()
{
    running = true;
    add_event(new CmdOutput(10, 10, title_string));
	add_event(new CmdDisp("Based God Interactive Fiction - a Dooby plot."));
    add_event(new CmdDisp("Copyright (c) 2016 moraine"));
    add_event(new CmdDisp("All rights reserved."));
    add_event(new CmdPause());
    add_event(new CmdDisp("-You wake."));
    add_event(new CmdPause());
    add_event(new CmdDisp("-No canine utterances grace your ears,\n and you can smell no fresh bacon cooking in the kitchen."));
    add_event(new CmdPause());
}

void GameStateIntro::cleanup()
{
}

void GameStateIntro::handle_event(Event* event)
{
    if(event->type == Event::CMD_PAUSE)
        paused = true;
    terminal->handle_event(event);
    if(mailbox.empty())
        running = false;
}

void GameStateIntro::run(sf::Time dt)
{

}

void GameStateIntro::draw(sf::RenderTarget* target)
{
    terminal->draw(target);
}
