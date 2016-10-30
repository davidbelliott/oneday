#include "Engine.h"
#include "GameState.h"
#include "Room.h"
#include "World.h"
#include "Terminal.h"
#include "common.h"
#include <iostream>

Engine::Engine()
:   EventSource(),
    paused(false),
    running(true),
    terminal(nullptr)
{
    terminal = new Terminal(this);
}


Engine::~Engine()
{
    delete terminal;
}

void Engine::push_state(GameState* state)
{
    game_states.push_back(state);
    state->init();
}

void Engine::pop_state()
{
    if(!game_states.empty())
    {
        game_states.back()->cleanup();
        game_states.pop_back();
    }
}

void Engine::push_event(Event* event)
{
    std::cout<<event->type<<std::endl;
    if(paused)
    {
        if(event->type == Event::KEY_PRESSED)
        {
            paused = false;
            ignore_next_event = true;
        }
    }
    else if(ignore_next_event)
        ignore_next_event = false;
    else
        EventSource::push_event(event);
}

void Engine::handle_event(Event* event)
{
    if(!paused && event->type == Event::CMD_PAUSE)
        pause();
}

void Engine::handle_events()
{
    while(!paused && !incoming_queue.empty())
    {
        Event* front_event = incoming_queue.front();
        incoming_queue.pop();
        handle_event(front_event);
        send_event(front_event);
    }
}

void Engine::get_input()
{
    terminal->get_input(this);
}

void Engine::draw()
{
    if(!game_states.empty())
        game_states.back()->draw();
    terminal->draw();
}

void Engine::run(sf::Time dt)
{
    if(!game_states.empty())
        game_states.back()->run(dt);
    while(game_states.size() > 0 && !game_states.back()->running)
        pop_state();
    if(game_states.size() == 0)
        running = false;
}

void Engine::disp(std::string t)
{
    CmdDisp* disp_cmd = new CmdDisp(t);
    push_event(disp_cmd);
}

void Engine::pause()
{
    if(!paused)
    {
        paused = true;
    }
}

void Engine::unpause()
{
    if(paused)
    {
        paused = false;
        CmdUnpause* cmd_unpause = new CmdUnpause();
        push_event(cmd_unpause);
    }
}
