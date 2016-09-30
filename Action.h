#pragma once

class Object;
class World;
class Terminal;

#include "Word.h"

#include <map>
#include <string>
#include <vector>


class Action
{
public:
	virtual ~Action();
	Action(Word name_in);


	virtual void add_object(Object* object_in);
	virtual void add_preposition(Word preposition_in);
	virtual void run(World* w, Terminal* t);
	
	Word name;
protected:

	virtual void act(World* w, Terminal* t, Object* o);

	
	
	std::vector<Object*> objects;
	std::vector<Word> prepositions;
};

class ActionGo : public Action { public: ActionGo(Word name_in) : Action(name_in) {}; void act(World* w, Terminal* t, Object* o); };
class ActionLook : public Action { public: ActionLook(Word name_in) : Action(name_in) {}; void act(World* w, Terminal* t, Object* o); };
class ActionQuit : public Action { public: ActionQuit(Word name_in) : Action(name_in) {}; void act(World* w, Terminal* t, Object* o); };
class ActionTake : public Action { public: ActionTake(Word name_in) : Action(name_in) {}; void act(World* w, Terminal* t, Object* o); };
class ActionWear : public Action { public: ActionWear(Word name_in) : Action(name_in) {}; void act(World* w, Terminal* t, Object* o); };
/*class ActionThrow : public Action { public: ActionThrow(); void act(World* w, Terminal* t); };*/
class ActionOpenContainer : public Action { public: ActionOpenContainer(Word name_in) : Action(name_in) {}; void act(World* w, Terminal* t, Object* o); };
class ActionHit : public Action { public: ActionHit(Word name_in) : Action(name_in) {}; void act(World* w, Terminal* t, Object* o); };
/*class ActionOpenDoor : public Action { public: ActionOpenDoor(); void act(World* w, Terminal* t); };
class ActionRead : public Action { public: ActionRead(); void act(World* w, Terminal* t); };
class ActionDrop : public Action { public: ActionDrop(); void act(World* w, Terminal* t); };
class ActionPutInContainer : public Action { public: ActionPutInContainer(); void act(World* w, Terminal* t); };
class ActionTurnOn : public Action { public: ActionTurnOn(); void act(World* w, Terminal* t); };
class ActionTurnOff : public Action { public: ActionTurnOff(); void act(World* w, Terminal* t); };
class ActionMove : public Action { public: ActionMove(); void act(World* w, Terminal* t); };
class ActionExamine : public Action { public: ActionExamine(); void act(World* w, Terminal* t); };
class ActionInventory : public Action { public: ActionInventory(); void act(World* w, Terminal* t); };
class ActionEat : public Action { public: ActionEat(); void act(World* w, Terminal* t); };
class ActionDrink : public Action { public: ActionDrink(); void act(World* w, Terminal* t); };*/
class ActionShout : public Action { public: ActionShout(Word name_in) : Action(name_in) {}; void act(World* w, Terminal* t, Object* o); };
/*class ActionCloseDoor : public Action { public: ActionCloseDoor(); void act(World* w, Terminal* t); };
class ActionBreak : public Action { public: ActionBreak(); void act(World* w, Terminal* t); };
class ActionBlessup : public Action { public: ActionBlessup(); void act(World* w, Terminal* t); };*/

