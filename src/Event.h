#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class Event
{
public:
	enum EventType
	{
        UPDATE,         // The engine is updating
        DRAW,           // The engine is drawing
        KEY_PRESSED,
        KEY_RELEASED,
        TEXT_ENTERED,
        USER_LINE,
        PRE_COMMAND,   // A command will be executed on this object
        POST_COMMAND
	};

	EventType type;

	Event(EventType type_in)
	: type(type_in)
    {}
	virtual ~Event()
	{}
};

typedef std::shared_ptr<Event> event_ptr;

class EventUpdate : public Event
{
    public:
        sf::Time dt;
        
        EventUpdate(sf::Time dt_in)
            : Event(UPDATE),
              dt(dt_in)
    {}
};

class EventDraw : public Event
{
    public:
        sf::RenderTarget* target;

        EventDraw(sf::RenderTarget* target_in)
            : Event(DRAW),
              target(target_in)
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

class EventKeyReleased : public Event
{
    public:
        sf::Keyboard::Key code;

        EventKeyReleased(sf::Keyboard::Key code_in)
            : Event(KEY_RELEASED),
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
