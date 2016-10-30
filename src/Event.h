#pragma once

class GameStateTerminal;
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
		CMD_INPUT,
		CMD_PAUSE,
        CMD_UNPAUSE,
		CMD_CHANGE_ROOM
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

class CmdInput : public Event
{
    public:
        CmdInput()
            : Event(CMD_INPUT)
        {}
};

class CmdUnpause : public Event
{
    public:
        CmdUnpause()
            : Event(CMD_UNPAUSE)
        {}
};
