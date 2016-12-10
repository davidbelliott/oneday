#include "GameStateTryAgainMenu.h"
#include "GameStateThugFight.h"
#include "Parser.h"
#include "Terminal.h"

GameStateTryAgainMenu::GameStateTryAgainMenu(Engine* engine_in)
    : GameState(engine_in)
{

}

GameStateTryAgainMenu::~GameStateTryAgainMenu()
{

}

void GameStateTryAgainMenu::init()
{
    send(std::make_shared<CmdDisp>("Try again? (y/n)"));
    send(std::make_shared<CmdInput>());
}

void GameStateTryAgainMenu::cleanup()
{

}

void GameStateTryAgainMenu::notify(event_ptr event)
{
    if(event->type == Event::USER_LINE)
    {
        std::string statement = std::static_pointer_cast<EventUserLine>(event)->line;
        arg_list args = {};
        if(matches(statement, "y", args))
        {
            send(std::make_shared<CmdRemoveGameState>(this));
            send(std::make_shared<CmdAddGameState>(new GameStateThugFight(engine)));
        }
        else if(matches(statement, "n", args))
        {
            send(std::make_shared<CmdQuit>());
        }
        else
        {
            send(std::make_shared<CmdDisp>("I don't understand that. Please type y (yes) or n (no)."));
            send(std::make_shared<CmdInput>());
        }
    }
    terminal->notify(event);
}
