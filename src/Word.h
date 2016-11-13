#pragma once
#include <string>
#include <vector>
class WordList;
class Word
{
private:

public:

    std::string word;
    std::vector<std::string> aliases;

	enum PartOfSpeech
	{
		ACTION,
		OBJECT,
		ACTION_MODIFIER,
		OBJECT_MODIFIER,
		UNRECOGNIZED
	};

    enum WordId
    {
        LOOK
    };

	Word(std::string word_in, std::vector<std::string> aliases_in, PartOfSpeech part_of_speech_in);
	Word();
	virtual ~Word();

	PartOfSpeech get_part_of_speech();
	void set_part_of_speech(PartOfSpeech part_of_speech_in);

	PartOfSpeech part_of_speech;
	int id;
	WordList* parent_list;
};

