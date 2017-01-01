#include "GameState.h"
#include "World.h"

GameState::GameState(Engine* engine_in)
:   Observer(),
    commands(),
    commands_priority(),
    last_command_result(""),
    engine(engine_in),
    paused(false),
    running(true),
    world(nullptr)
{
}


GameState::~GameState()
{
}

void GameState::init()
{
    last_command_result = "";
    paused = false;
    running = true;
}

void GameState::cleanup()
{

}

void GameState::notify(event_ptr event)
{
    if(paused)
    {
        if(event->type == Event::KEY_PRESSED &&
                std::static_pointer_cast<EventKeyPressed>(event)->code == sf::Keyboard::Return)
            unpause();
    }
}

void GameState::send(cmd_ptr cmd)
{
    commands.push(cmd);
}

void GameState::send_front(cmd_ptr cmd)
{
    commands_priority.push(cmd);
}

void GameState::execute(cmd_ptr cmd)
{
    cmd->run_and_callback(this);
}

bool GameState::execute_front_command()
{
    if(!paused && (!commands_priority.empty() || !commands.empty()))
    {
        cmd_ptr cmd = nullptr;
        if(!commands_priority.empty())
        {
            cmd = commands_priority.front();
            commands_priority.pop();
        }
        else if(!commands.empty())
        {
            cmd = commands.front();
            commands.pop();
        }
        execute(cmd);
    }
    return (!paused && (!commands_priority.empty() || !commands.empty()));
}

void GameState::update(sf::Time dt)
{

}

void GameState::draw()
{
}

void GameState::pause()
{
    paused = true;
}

void GameState::unpause()
{
    paused = false;
}
