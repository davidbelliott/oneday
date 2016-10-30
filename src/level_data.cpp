#include "Action.h"
#include "WordList.h"
#include "Engine.h"
#include "Player.h"
#include "World.h"
#include "Room.h"
#include "common.h"
#include "CmdDisp.h"
#include "CmdPause.h"
#include "level_data.h"
#include <SFML/Audio.hpp>
#include <iostream>

World* generate_world()
{
	World* world = new World();
	world->flags =
	{
		{ "alive", 1 },
		{ "health", 100 },
		{ "woke_up", 0 }
	};
	world->cur_room = "jamal_bedroom";
    {
        Player* player = new Player("Jamal", "a sturdy creature fond of drink and industry");
        world->player = player;
    }
	{
		Room* jamal_bedroom = new Room();
		jamal_bedroom->set_name("jamal_bedroom");
        jamal_bedroom->pretty_name = "Jamal's bedroom";
		jamal_bedroom->shallow_description = "The walls of this cluttered hovel are plastered with layers of grime and old posters.";
		jamal_bedroom->directions[EAST] = "jamal_corridor";
		jamal_bedroom->directions[SOUTH] = "jamal_bathroom";
		jamal_bedroom->pre_action = [](World* w, Engine* e, Action* a, Object* o)
		{
			if (a->name.id == a->name.parent_list->LOOK && !o->get_flag("woke_up"))
			{
				e->push_event(new CmdDisp("You wake.\nNo canine utterances grace your ears, and you can smell no fresh bacon cooking in the kitchen."));
                w->player->set_objective("Get outta dis crib", e);
				o->set_flag("woke_up", 1);
                e->push_event(new CmdPause());
                w->good_day.play();
			}
            return true;
		};

		Object* window = new Object("window", "A single smeared window to the north suffuses the room in dim light.");
		window->deep_description = "Looking through the window, you notice a gang of thugs gathered in front of your house.";
		jamal_bedroom->add_child(window);

        Object* paper = new Object("paper", "A crumpled sheeit of paper lies on the floor.");
        paper->name.aliases = { "sheet", "sheeit" };
        paper->properties |= Object::READABLE | Object::TAKEABLE;
        paper->readable_data = "Just waking up in the morning, gotta thank God\nI don't know but today seems kinda odd\nNo barking from the dog, no smog\nAnd momma cooked a breakfast with no hog.";
        paper->readable_data += "\nY'all cowards type help, pls.";
        jamal_bedroom->add_child(paper);

		world->add_child(jamal_bedroom);
	}

	return world;
}
