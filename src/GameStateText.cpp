#include "GameStateText.h"
#include "Engine.h"
#include "Terminal.h"
#include "World.h"

GameStateText::GameStateText(Engine* engine_in)
    : GameState(engine_in),
      parser(new Parser())
{
}

GameStateText::~GameStateText()
{
    delete parser;
}
/*
void GameStateText::init()
{
    running = true;
    parser = new Parser();
	engine->get_world()->set_current_room(world->get_current_room()->name.word, this);
    add_event(std::make_shared<CmdInput>());
}*/

void GameStateText::notify(event_ptr event)
{
    if(paused)
    {
        if(event->type == Event::KEY_PRESSED)
            unpause();
    }
    else if(event->type == Event::USER_LINE)
    {
        std::string line = std::static_pointer_cast<EventUserLine>(event)->line;
        if(line == "")
            send(std::make_shared<CmdDisp>("Please enter a command."));
        else
        {
            cmd_ptr cmd = parser->parse(line, engine->world);
            if(cmd)
                send(cmd);
            else
                send(std::make_shared<CmdDisp>("I don't understand."));
        }
        send(std::make_shared<CmdInput>());
    }
    terminal->notify(event);
}
