#include "GameStateIntro.h"
#include "Event.h"
#include "Engine.h"
#include "Config.h"
#include "Terminal.h"

GameStateIntro::GameStateIntro(Engine* engine_in)
    : GameState(engine_in),
    title_string("ONE DAY IN THE LIFE OF YOUNG JAMAL")
{
    running = true;
    int center_x = (config::screen_w_chars - title_string.length()) / 2;
    int center_y = (config::screen_h_chars - 1) / 2;
    send(std::make_shared<CmdOutput>(center_x, center_y, title_string));
    send(std::make_shared<CmdPause>());
    send(std::make_shared<CmdClear>());
    send(std::make_shared<CmdDisp>("-You wake."));
    send(std::make_shared<CmdPause>());
    send(std::make_shared<CmdDisp>("-No canine utterances grace your ears,\n and you can smell no fresh bacon cooking in the kitchen."));
    send(std::make_shared<CmdPause>());
}

GameStateIntro::~GameStateIntro()
{
}

void GameStateIntro::update(sf::Time dt)
{
    if(!paused && commands.empty())
        running = false;
}
