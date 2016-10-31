#include "Action.h"
#include "WordList.h"
#include "Engine.h"
#include "Player.h"
#include "World.h"
#include "Room.h"
#include "common.h"
#include "level_data.h"
#include "Receiver.h"
#include "Event.h"
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

	{
		Room* jamal_bathroom = new Room();
		jamal_bathroom->set_name("jamal_bathroom");
        jamal_bathroom->pretty_name = "Jamal's bathroom";
		jamal_bathroom->shallow_description = "This is where you defecate daily. This cesuo is a reeking pigsty.\nOne door leads to the north.";
		jamal_bathroom->directions[NORTH] = "jamal_bedroom";

		Object* hole = new Object("hole", "A dark hole gapes in the floor, presumably where a toilet used to be.");
		hole->properties |= Object::GOABLE;
		hole->goable_data = "sewer";
		hole->pre_action = [](World* w, Receiver* r, Action* a, Object* o)
		{
			if (a->name.id == a->name.parent_list->GO)
			{
				if (w->player->clothing == "hazmat")
				{
					r->add_event(new CmdDisp("With the hazmat suit on, you tentatively step down into the hole and lower yourself into the murky water.\nIt rises gurgling to your neck.\nWith a desperate resignation, you plunge beneath the dark sewage."));
                    r->add_event(new CmdPause());
                    return true;
				}
				else
				{
					r->add_event(new CmdDisp("The hole is full of disgusting sewage water. You want to touch it with your bare skin? Kimochi warui~"));
                    return false;
				}
			}
		};
		jamal_bathroom->add_child(hole);

		world->add_child(jamal_bathroom);
	}

	{
		Room* jamal_corridor = new Room("jamal_corridor", "the Magick corridor", "This hallway is imbued with a strong Faerie Magick.");
        jamal_corridor->directions[NORTH] = "jamal_house_block";
		jamal_corridor->directions[SOUTH] = "jamal_staircase";
		jamal_corridor->directions[WEST] = "jamal_bedroom";

        jamal_corridor->pre_action = [](World* w, Receiver* r, Action* a, Object* o)
        {
            if(a->name.id == a->name.parent_list->GO)
            {
                if(!a->prepositions.empty() && a->prepositions[0].word == "north")
                {
                    r->add_event(new CmdDisp("You hear the intense rustling of thugs lying in wait outside your door. Best not go out this way."));
                    return false;
                }
            }
            return true;
        };

		world->add_child(jamal_corridor);
	}

	{
		Room* jamal_house_block = new Room("jamal_house_block", "Jamal's front yard", "As you open the door, an angry mob of thugs accosts you. They drag you to the ground and kill you. You die, killed by the angry mob of thugs.");
		
		world->add_child(jamal_house_block);
	}

	{
		Room* jamal_staircase = new Room("jamal_staircase", "the staircase", "A rickety staircase descending into the basement.");
		jamal_staircase->directions[UP] = "jamal_corridor";
		jamal_staircase->directions[DOWN] = "henrik_library";

		world->add_child(jamal_staircase);
	}

	{
		Room* henrik_library = new Room("henrik_library", "Henrik's library", "This subterranean den is where the father of realism does his stuff.");
		henrik_library->directions[NORTH] = "jamal_staircase";
		Object* shelf = new Object("bookshelf", "A massive bookshelf covers the wall to the west.");
        shelf->name.aliases = { "shelf" };
		shelf->deep_description = "The shelf is full of books written by dead white men.";
		henrik_library->add_child(shelf);
		Object* book = new Object("book", "One book protrudes farther than the rest.");
		book->properties |= Object::TAKEABLE;
		book->deep_description = "The book is Ghosts, by Henrik Ibsen.";
		shelf->add_child(book);
		Object* secret_switch = new Object("switch", "A secret switch protrudes from the empty slot where a book used to be.");
		secret_switch->properties = Object::HITTABLE;
		secret_switch->post_action = [=](World* w, Receiver* r, Action* a, Object* o)
		{
			if (a->name.id == a->name.parent_list->HIT)
			{
				r->add_event(new CmdDisp("Hitting the switch causes the bookshelf to slide to the side, revealing a doorway leading to the west."));
				shelf->shallow_description = "A massive bookshelf is slid to one side of the west wall.";
				henrik_library->directions[WEST] = "henrik_lab";
			}
            return true;
		};
		henrik_library->add_child(secret_switch);
		book->post_action = [=](World* w, Receiver* r, Action* a, Object* o)
		{
			if (a->name.id == a->name.parent_list->TAKE)
			{
			    r->add_event(new CmdDisp("Taking the book reveals a secret switch."));
				shelf->shallow_description += " One book is missing, leaving an empty slot.";
				shelf->deep_description += " One book is missing, leaving an empty slot.";
				secret_switch->properties |= Object::VISIBLE;
			}
            return true;
		};
		world->add_child(henrik_library);
	}

	{
		Room* henrik_lab = new Room("henrik_lab", "Henrik\'s lab", "An underground laboratory where Henrik does his stuff.\nA door leads out to the east.");
		henrik_lab->directions[EAST] = "henrik_library";
		world->add_child(henrik_lab);

		Object* hazmat_suit = new Object("hazmat", "A hazmat suit lies crumpled on the ground.");
		hazmat_suit->name.aliases = { "suit" };
		hazmat_suit->deep_description = "This frumpy apparel doesn't suit your style.";
		hazmat_suit->properties |= Object::WEARABLE;
		hazmat_suit->properties |= Object::TAKEABLE;
		henrik_lab->add_child(hazmat_suit);
	}

	{
		Room* sewer = new Room("sewer", "the sewer", "A slimy underground pipe, going to the west. Sewage runs thickly around your calves as you wade inna water.");
		sewer->directions[WEST] = "main_street";
        world->add_child(sewer);
	}

    {
        Room* main_street = new Room("main_street", "Hoover Street", "A desolate wind blows the asphalt.");
        main_street->directions[WEST] = "club";
        main_street->directions[SOUTH] = "bus_stop";
        world->add_child(main_street);

        Object* sewer_drain = new Object("sewer drain", "A sewer drain lies open to the right of the road.");
        sewer_drain->name.aliases = { "sewer", "drain" };
        sewer_drain->properties |= Object::GOABLE;
        sewer_drain->goable_data = "sewer";
        main_street->add_child(sewer_drain);

        Object* sam = new Object("urban youth", "An urban youth stands on the street corner.");
        sam->name.aliases = { "youth", "man", "person", "sam" };
        sam->deep_description = "He has been unhappy lately. He slept without a proper room recently. He admired a fine Sloot lately.\nHe is tall. His eyes are hazel. His extraordinarily broad tall ears have great swinging lobes. His nose is broad. His skin is ebony.";
        sam->deep_description += "\nA sturdy creature fond of drink.";
        sam->properties |= Object::TALKABLE;
        sam->talkable_data = { "-What are you standing here for?", 
            "-I'm here for da people, Favio. Do you understand?",
            "-What do you mean?",
            "-Favio, examining into antiquity, we find that Big L was styled Fang-xun.",
            " He was reverential, intelligent, accomplished, and thoughtful--naturally and without effort.",
            " However, we, like kings, were led astray. We were kept down by the Man, and none but the president can deliver.",
            "Shocked by this revelation, you stammer:",
            "-Th-that's horrible! How can I change this?",
            "-Now listen here, young feller. You must visit the president.",
            "-But how?",
            "-Viper will host a rap contest at the Club. His offer? A trip to the White House to meet the president.",
            "You gasp. Such great opportunity--but at such a price!",
            " Go, Favio. Be not afraid.",
            " Do it for da people.",
            "The man disappears in a puff of smoke.",
            "Your objective: get to the Club and out-rap Viper." };
        sam->post_action = [](World* w, Receiver* r, Action* a, Object* o) {
            if(a->name.id == a->name.parent_list->TALK_TO)
                o->properties = 0;
            return true;
        };

        main_street->add_child(sam);
    }

	return world;
}
