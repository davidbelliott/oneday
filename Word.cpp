#include "Word.h"

Word::Word(std::string word_in, std::vector<std::string> aliases_in, PartOfSpeech part_of_speech_in)
: word(word_in), aliases(aliases_in), part_of_speech(part_of_speech_in), id(-1), parent_list(NULL)
{
}

Word::Word()
	: Word("", {}, Word::UNRECOGNIZED)
{
}

Word::~Word()
{
}

Word::PartOfSpeech Word::get_part_of_speech()
{
	return part_of_speech;
}

void Word::set_part_of_speech(PartOfSpeech part_of_speech_in)
{
	part_of_speech = part_of_speech_in;
}
