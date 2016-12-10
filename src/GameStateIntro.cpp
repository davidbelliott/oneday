#include "GameStateIntro.h"
#include "Event.h"
#include "Engine.h"
#include "Config.h"
#include "Terminal.h"
#include "File.h"

GameStateIntro::GameStateIntro(Engine* engine_in)
    : GameState(engine_in),
    title_string(get_file_contents("jamal.txt"))
{
    running = true;
    std::string title_string(get_file_contents("jamal.txt"));
    running = true;
    send(std::make_shared<CmdOutput>(0, 0, "One Day in the Life of"));
    std::shared_ptr<CmdOutput> output = std::make_shared<CmdOutput>(0, 1, title_string);
    output->spread = 100;
    send(output);
    send(std::make_shared<CmdDisp>("\n\n\n\n\n\n\n"));
    send(std::make_shared<CmdDisp>("You wake."));
    send(std::make_shared<CmdDisp>("No canine utterances grace your ears,\nand you can smell no fresh bacon cooking in the kitchen."));
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

void GameStateIntro::draw(sf::RenderTarget* target)
{
    terminal->draw(target);
}
