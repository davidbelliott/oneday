#include "GameStateIntro.h"
#include "Event.h"
#include "Engine.h"
#include "Config.h"

GameStateIntro::GameStateIntro(Engine* engine_in)
    : GameState(engine_in),
    terminal(),
    title_string("ONE DAY IN THE LIFE OF YOUNG JAMAL")
{
}

GameStateIntro::~GameStateIntro()
{
}

void GameStateIntro::init()
{
    running = true;
    int center_x = (config::screen_w_chars - title_string.length()) / 2;
    int center_y = (config::screen_h_chars - 1) / 2;
    add_event(std::make_shared<CmdOutput>(center_x, center_y, title_string));
    add_event(std::make_shared<CmdPause>());
    add_event(std::make_shared<CmdClear>());
    add_event(std::make_shared<CmdDisp>("-You wake."));
    add_event(std::make_shared<CmdPause>());
    add_event(std::make_shared<CmdDisp>("-No canine utterances grace your ears,\n and you can smell no fresh bacon cooking in the kitchen."));
    add_event(std::make_shared<CmdPause>());
}

void GameStateIntro::cleanup()
{
}

void GameStateIntro::handle_event(std::shared_ptr<Event> event)
{
    if(event->type == Event::CMD_PAUSE)
        paused = true;
    terminal.handle_event(event);
    if(mailbox.empty())
        running = false;
}

void GameStateIntro::run(sf::Time dt)
{

}

void GameStateIntro::draw(sf::RenderTarget* target)
{
    terminal.draw(target);
}
