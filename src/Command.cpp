Command::Command(CommandType type_in)
    : type(type_in)
{
}

Command::~Command()
{
}

void Command::run(GameState* g)
{
}

void Command::add_object(Object* o)
{
    objects.push_back(o);
}

CmdDisp::CmdDisp(std::string str_in)
:   Command(DISP),
    str(str_in)
{
}

CmdDisp::run(GameState* g)
{
    g->terminal->disp(str);
}

CmdOutput::CmdOutput(int x_in, int y_in, std::string str_in)
    : Command(CMD_OUTPUT),
    x(x_in),
    y(y_in),
    str(str_in)
{
}

void CmdOutput::run(GameState* g)
{
    g->terminal->output(x, y, str);
}

CmdClear::CmdClear()
    : Command(CMD_CLEAR)
{}

void CmdClear::run(GameState* g)
{
    g->terminal->clr();
}

CmdSetColor::CmdSetColor(sf::Color color_in)
    : Command(CMD_SETCOLOR),
      color(color_in)
{
}

void CmdSetColor::run(GameState* g)
{
    g->terminal->set_color(color);
}

CmdInput::CmdInput()
    : Command(CMD_INPUT)
{}

void CmdInput::run(GameState* g)
{
}

CmdPause::CmdPause()
    : Command(CMD_PAUSE)
{}

void CmdPause::run(GameState* g)
{
    g->pause();
}

CmdUnpause::CmdUnpause()
    : Command(CMD_UNPAUSE)
{}

void CmdUnpause::run(GameState* g)
{
    g->unpause();
}

CmdSetObjective::CmdSetObjective(std::string objective_in)
    : Command(CMD_SET_OBJECTIVE),
      objective(objective_in)
{}

void CmdSetObjective::run(GameState* g)
{

}

CmdAddGameState::CmdAddGameState(GameState* state_to_add_in)
    : Command(CMD_ADD_GAMESTATE),
      state_to_add(state_to_add_in)
{}

void CmdAddGameState::run(GameState* g)
{

}

CmdSetRoom::CmdSetRoom(World* world_in, std::string new_room_in)
    : Command(CMD_SET_ROOM),
      world(world_in),
      new_room(new_room_in)
{
}

void CmdSetRoom::run(GameState* g)
{
    world->set_room(new_room);
}

CmdDescribe::CmdDescribe()
    : Command(CMD_DESCRIBE)
{
}

void CmdDescribe::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
        objects[i]->describe(g, true, true);
}

CmdQuit::CmdQuit()
    : Command(CMD_QUIT)
{
}

void CmdQuit::run(GameState* g)
{
    g->engine->running = false;
}

CmdTake::CmdTake()
    : Command(CMD_TAKE)
{
}

void CmdTake::run(GameState* g)
{
    
}

CmdWear::CmdWear()
    : Command(CMD_WEAR)
{
}

void CmdWear::run(GameState* g)
{

}

CmdHit::CmdHit()
    : Command(CMD_HIT)
{
}

void CmdHit::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        Object* o = objects[i];

        if (o->properties & Object::HITTABLE)
        {
            if (o->flipped)
                r->add_event(std::make_shared<CmdDisp>("The " + o->name.word + " has already been hit."));
            else
            {
                r->add_event(std::make_shared<CmdDisp>("You hit the " + o->name.word + "."));
                o->flipped = true;
            }
        }
        else if (o)
        {
            r->add_event(std::make_shared<CmdDisp>("You can't hit the " + o->name.word));
        }
    }
}

CmdShout::CmdShout()
    : Command(CMD_SHOUT)
{
}

void CmdShout::run(GameState* g)
{
    g->terminal->disp("-SHEEEEIT!");
}

CmdRead::CmdRead()
    : Command(CMD_READ)
{
}

void CmdRead::run(GameState* g)
{
    for(int i = 0; i < objects.size(); i++)
    {
        if(objects[i]->properties & Object::READABLE)
        {
            g->terminal->disp("The " + objects[i]->name.word + " reads:");
            g->terminal->disp(objects[i]->readable_data);
        }
    }
}

CmdTalkTo::CmdTalkTo()
    : Command(CMD_TALK_TO)
{
}

void CmdTalkTo::run(GameState* g)
{
    for(int i = 0; i < objects[i].size(); i++)
    {
        Object* o = objects[i];
        if(o->properties & Object::TALKABLE)
        {
            for(int j = 0; j < o->talkable_data.size(); j++)
            {
                g->send(std::make_shared<CmdDisp>(o->talkable_data[i]));
                o->send(std::make_shared<CmdPause>());
            }
        }
    }
}

CmdHelp::CmdHelp()
    : Command(CMD_HELP)
{
}

void CmdHelp::run(GameState* g)
{

}
