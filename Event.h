#pragma once

class GameStateTerminal;
#include <SFML/Graphics.hpp>
#include <functional>

class Event
{
public:
	enum EventType
	{
		SFML,
		CMD_DISP,
		CMD_OUTPUT,
		CMD_INPUT,
		CMD_PAUSE,
		CMD_CHANGE_ROOM
	};

	struct SfmlEventData
	{
		sf::Event sf_event;
	};

	struct CmdDispEventData
	{
		std::string* string;
		bool wrap = true;
	};

	struct CmdOutputEventData
	{
		std::string* string;
		int x;
		int y;
		bool wrap = true;
	};

	struct CmdInputEventData
	{
		std::function<void (std::string, GameStateTerminal*)>* callback;
	};

	struct CmdPauseEventData
	{

	};

	struct CmdChangeRoomEventData
	{
		std::string* new_room;
	};

	EventType type;

	union
	{
		SfmlEventData sfml_event_data;
		CmdDispEventData cmd_disp_event_data;
		CmdOutputEventData cmd_output_event_data;
		CmdInputEventData cmd_input_event_data;
		CmdPauseEventData cmd_pause_event_data;
		CmdChangeRoomEventData cmd_change_room_event_data;
	};

	Event()
	{}
	virtual ~Event()
	{}
};