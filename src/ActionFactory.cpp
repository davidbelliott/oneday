#include "ActionFactory.h"
#include "WordList.h"


ActionFactory::ActionFactory()
{
}


ActionFactory::~ActionFactory()
{
}

Action* ActionFactory::create_action(Word word)
{
	WordList* l = word.parent_list;
	if (l)
	{
		if (word.id == l->GO) return new ActionGo(word);
		else if (word.id == l->LOOK) return new ActionLook(word);
		else if (word.id == l->QUIT) return new ActionQuit(word);
		else if (word.id == l->TAKE) return new ActionTake(word);
		else if (word.id == l->WEAR) return new ActionWear(word);
		/*else if (action_type == Action::THROW) return new ActionThrow();*/
		else if (word.id == l->HIT) return new ActionHit(word);
		else if (word.id == l->OPEN_CONTAINER) return new ActionOpenContainer(word);
		/*else if (action_type == Action::OPEN_DOOR) return new ActionOpenDoor();*/
		else if (word.id == l->READ) return new ActionRead(word);
		/*else if (action_type == Action::DROP) return new ActionDrop();
		else if (action_type == Action::PUT_IN_CONTAINER) return new ActionPutInContainer();
		else if (action_type == Action::TURN_ON) return new ActionTurnOn();
		else if (action_type == Action::TURN_OFF) return new ActionTurnOff();
		else if (action_type == Action::MOVE) return new ActionMove();
		else if (action_type == Action::EXAMINE) return new ActionExamine();
		else if (action_type == Action::INVENTORY) return new ActionInventory();
		else if (action_type == Action::EAT) return new ActionEat();
		else if (action_type == Action::DRINK) return new ActionDrink();*/
		else if (word.id == l->SHOUT) return new ActionShout(word);
		/*else if (action_type == Action::CLOSE_DOOR) return new ActionCloseDoor();
		else if (action_type == Action::BREAK) return new ActionBreak();
		else if (action_type == Action::BLESSUP) return new ActionBlessup();*/
        else if (word.id == l->TALK_TO) return new ActionTalkTo(word);
        else if (word.id == l->HELP) return new ActionHelp(word);
	}
	return NULL;
}
