#pragma once
#include "TextReferencable.h"
#include <string>
class WordList;
class Word : public TextReferencable
{
private:

public:


	enum PartOfSpeech
	{
		ACTION,
		OBJECT,
		ACTION_MODIFIER,
		OBJECT_MODIFIER,
		UNRECOGNIZED
	};

	Word(std::string name_in, std::vector<std::string> aliases_in, PartOfSpeech part_of_speech_in);
	Word();
	virtual ~Word();

	PartOfSpeech get_part_of_speech();
	void set_part_of_speech(PartOfSpeech part_of_speech_in);

	PartOfSpeech part_of_speech;
	int id;
	WordList* parent_list;
};

