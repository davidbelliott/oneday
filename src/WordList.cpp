#include "WordList.h"



WordList::WordList()
	: N_WORDS(0),
	GO(add_word({"go", {}, Word::ACTION })),
	LOOK(add_word({"look", {"examine"}, Word::ACTION})),
	QUIT(add_word({ "quit", {"q"}, Word::ACTION })),
	TAKE(add_word({ "take", { "grab", "snatch", "get", "obtain", "pick", "remove" }, Word::ACTION })),
	WEAR(add_word({"wear", {"don", "put"}, Word::ACTION})),
	THROW(add_word({ "throw", { "toss", "hurl" }, Word::ACTION })),
	HIT(add_word({ "hit", { "flip", "turn" }, Word::ACTION })),
	OPEN_CONTAINER(add_word({ "open", { }, Word::ACTION })),
	OPEN_DOOR(add_word({ "open_door", { }, Word::ACTION })),
	READ(add_word({ "read", { "scan", "peruse" }, Word::ACTION })),
	DROP(add_word({ "drop", { "discard" }, Word::ACTION })),
	PUT_IN_CONTAINER(add_word({ "put", { }, Word::ACTION })),
	TURN_ON(add_word({ "turn on", { }, Word::ACTION })),
	TURN_OFF(add_word({ "turn off", { }, Word::ACTION })),
	MOVE(add_word({ "move", { "pull" }, Word::ACTION })),
	INVENTORY(add_word({ "inventory", { "i" }, Word::ACTION })),
	EAT(add_word({ "eat", { }, Word::ACTION })),
	DRINK(add_word({ "drink", { }, Word::ACTION })),
	SHOUT(add_word({ "shout", { "scream", "yell", "holler" }, Word::ACTION })),
	CLOSE_DOOR(add_word({ "close", { "shut" }, Word::ACTION })),
	BREAK(add_word({ "break", { "destroy" }, Word::ACTION })),
	BLESSUP(add_word({ "bless up", { }, Word::ACTION })),
    TALK_TO(add_word({ "talk to", { "talk" }, Word::ACTION })),
    HELP(add_word({ "help", { "h" }, Word::ACTION })),
    ACTION_MAX(HELP + 1),

	//Action modifiers
	NORTH(add_word({ "north", { "n" }, Word::ACTION_MODIFIER })),
	EAST(add_word({ "east", { "e" }, Word::ACTION_MODIFIER })),
	SOUTH(add_word({ "south", { "s" }, Word::ACTION_MODIFIER })),
	WEST(add_word({ "west", { "w" }, Word::ACTION_MODIFIER })),
	UP(add_word({ "up", { }, Word::ACTION_MODIFIER })),
	DOWN(add_word({ "down", { }, Word::ACTION_MODIFIER }))
{
}


WordList::~WordList()
{
}

int WordList::add_word(Word word)
{
	word.id = N_WORDS;
	word.parent_list = this;
	words_by_name[word.word] = word;
	std::vector<std::string> aliases = word.aliases;
	for (auto it = aliases.begin(); it != aliases.end(); ++it)
	{
		words_by_name[*it] = word;
	}
	words_by_id[N_WORDS] = word;
	return N_WORDS++;
}

Word WordList::get_word(std::string name)
{
	if (words_by_name.count(name) > 0)
		return words_by_name[name];
	else
		return Word(name, {}, Word::UNRECOGNIZED);
}
