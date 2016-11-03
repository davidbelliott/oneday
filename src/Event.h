#pragma once

class GameStateTerminal;
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <functional>

class Event
{
public:
	enum EventType
	{
        KEY_PRESSED,
        TEXT_ENTERED,
        USER_LINE,
		CMD_DISP,
		CMD_OUTPUT,
        CMD_CLEAR,
        CMD_SETCOLOR,
		CMD_INPUT,
		CMD_PAUSE,
        CMD_UNPAUSE,
		CMD_CHANGE_ROOM,
        CMD_SET_OBJECTIVE
	};

	EventType type;

	Event(EventType type_in)
	: type(type_in)
    {}
	virtual ~Event()
	{}
};

class EventKeyPressed : public Event
{
    public:
        sf::Keyboard::Key code;

        EventKeyPressed(sf::Keyboard::Key code_in)
            : Event(KEY_PRESSED),
              code(code_in)
        {}
};

class EventTextEntered : public Event
{
    public:
        char c;

        EventTextEntered(char c_in)
            : Event(TEXT_ENTERED),
              c(c_in)
        {}
};

class EventUserLine : public Event
{
    public:
        std::string line;

        EventUserLine(std::string line_in)
            : Event(USER_LINE),
              line(line_in)
        {}
};

class CmdDisp : public Event
{
    public:
        std::string str;

        CmdDisp(std::string str_in)
        :   Event(CMD_DISP),
            str(str_in)
        {
        }
};

class CmdOutput : public Event
{
    public:
        int x;
        int y;
        std::string str;

        CmdOutput(int x_in, int y_in, std::string str_in)
            : Event(CMD_OUTPUT),
            x(x_in),
            y(y_in),
            str(str_in)
    {}
};

class CmdClear : public Event
{
    public:
        CmdClear()
            : Event(CMD_CLEAR)
        {}
};

class CmdSetColor : public Event
{
    public:
        sf::Color color;
        CmdSetColor(sf::Color color_in = config::colors[config::color_default_fg])
            : Event(CMD_SETCOLOR),
              color(color_in)
        {}
};

class CmdInput : public Event
{
    public:
        CmdInput()
            : Event(CMD_INPUT)
        {}
};

class CmdPause : public Event
{
    public:
        CmdPause()
            : Event(CMD_PAUSE)
        {}
};

class CmdUnpause : public Event
{
    public:
        CmdUnpause()
            : Event(CMD_UNPAUSE)
        {}
};

class CmdSetObjective : public Event
{
    public:
        std::string objective;
        CmdSetObjective(std::string objective_in)
            : Event(CMD_SET_OBJECTIVE),
              objective(objective_in)
    {}
};
