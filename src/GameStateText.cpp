#include "GameStateText.h"
#include "Engine.h"
#include "Terminal.h"
#include "World.h"
#include "File.h"

GameStateText::GameStateText(Engine* engine_in)
    : GameState(engine_in),
      parser(new Parser()),
      line("")
{
}

GameStateText::~GameStateText()
{
    delete parser;
}

void GameStateText::init()
{
    std::string title_string(get_file_contents("jamal.txt"));
    running = true;
    world->set_current_room(world->cur_room);
    /*std::shared_ptr<CmdOutput> output = std::make_shared<CmdOutput>(0, 1, title_string);
    output->spread = 100;
    send(output);
    send(std::make_shared<CmdOutput>(30, 8, "==IS ALSO NEARBY=="));
    send(std::make_shared<CmdDisp>("\n\n\n\n\n\n\n\n\n"));
    send(std::make_shared<CmdDisp>("You wake."));
    send(std::make_shared<CmdDisp>("No canine utterances grace your ears, and you can smell no fresh bacon cooking in the kitchen."));
    send(std::make_shared<CmdDisp>("Type 'look' to look around."));*/
    engine->terminal->disp(title_string);
    engine->terminal->disp("==IS ALSO NEARBY==");
    engine->terminal->disp("You wake.");
}

void GameStateText::cleanup()
{
}

void GameStateText::notify(event_ptr event)
{
}

void GameStateText::update(sf::Time dt)
{
    std::string line = engine->terminal->get_input();
    if(line == "")
        engine->terminal->disp("-say something, pls");
    else
    {
        Command* command = parser->parse(line, this);
        if(command)
            command->run_with_callbacks(this);
    }
}
