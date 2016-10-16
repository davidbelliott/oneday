#include "EventSource.h"
#include "Engine.h"
#include "GameState.h"
#include "Room.h"
#include "World.h"
#include "Terminal.h"
#include "common.h"

Engine::Engine(Terminal* terminal_in)
: paused(false), running(true), terminal(terminal_in)
{
}


Engine::~Engine()
{
}

void Engine::draw()
{
    game_states.back()->draw();
}

void Engine::run(sf::Time dt)
{
    game_states.back()->run(dt);
    if(!game_states.back()->running)
        running = false;
}
