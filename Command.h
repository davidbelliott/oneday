#pragma once
class Command
{
public:
	enum CommandType
	{
		DISP_TEXT,
		OUTPUT_TEXT,
		INPUT_TEXT,
		PAUSE
	};

	Command();
	virtual ~Command();
};

