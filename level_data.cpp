#include "Parser.h"
#include "Terminal.h"
#include "World.h"
#include "Room.h"
#include "common.h"
#include "level_data.h"

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
		Room* jamal_bedroom = new Room();
		jamal_bedroom->set_name("jamal_bedroom");
		jamal_bedroom->shallow_description = "Jamal's Bedroom\nThe walls of this cluttered hovel are plastered with layers of grime and old posters.";
		jamal_bedroom->directions[EAST] = "jamal_corridor";
		jamal_bedroom->directions[SOUTH] = "jamal_bathroom";
		jamal_bedroom->run_action = [](World* w, Terminal* t, Action* a, Object* o)
		{
			if (a->name.id == a->name.parent_list->LOOK && !o->get_flag("woke_up"))
			{
				t->disp("You wake up feeling destitute.\nNo canine utterances grace your ears, and you can smell no fresh bacon cooking in the kitchen.");
				o->set_flag("woke_up", 1);
				t->pause();
			}
			return true;
		};

		Object* window = new Object("window", "A single smeared window to the north suffuses the room in dim light.");
		window->deep_description = "Looking through the window, you notice a gang of thugs gathered in front of your house.";
		jamal_bedroom->add_child(window);

		Object* hall_door = new Object("west_door", "A dilapidated door hangs from the east wall on its hinges.");
		hall_door->properties |= Object::GOABLE;
		hall_door->goable_data = "jamal_corridor";
		jamal_bedroom->add_child(hall_door);

		Object* bath_door = new Object("south_door", "Another door leads to the south.");
		bath_door->properties |= Object::GOABLE;
		bath_door->deep_description = "The door is made of rusted metal.";
		bath_door->goable_data = "jamal_bathroom";
		jamal_bedroom->add_child(bath_door);

		world->add_child(jamal_bedroom);
	}

	{
		Room* jamal_bathroom = new Room();
		jamal_bathroom->set_name("jamal_bathroom");
		jamal_bathroom->shallow_description = "Jamal's Bathroom\nThis is where you defecate daily. This cesuo is a reeking pigsty.\nOne door leads to the north.";
		jamal_bathroom->directions[NORTH] = "jamal_bedroom";

		Object* hole = new Object("hole", "A dark hole gapes in the floor, presumably where a toilet used to be.");
		hole->properties |= Object::GOABLE;
		hole->goable_data = "sewer";
		hole->run_action = [](World* w, Terminal* t, Action* a, Object* o)
		{
			if (a->name.id == a->name.parent_list->GO)
			{
				if (w->clothing && w->clothing->name.word == "hazmat")
				{
					t->disp("With the hazmat suit on, you tentatively step down into the hole and lower yourself into the murky water.\nIt rises gurgling to your neck.\nWith a desperate resignation, you plunge beneath the dark sewage.");
					t->pause();
					return true;
				}
				else
				{
					t->disp("The hole is full of disgusting sewage water. You want to touch it with your bare skin? Kimochi warui~");
					return false;
				}
			}
			return true;
		};
		jamal_bathroom->add_child(hole);

		world->add_child(jamal_bathroom);
	}

	{
		Room* jamal_corridor = new Room("jamal_corridor", "Magick Corridor\nThis hallway is imbued with a strong Faerie Magick.\nIt runs from north to south.\nThe front door of the house exits to the north.\nOne door exits to the east, and one to the west.\nA series of bare bulbs dangle from the ceiling.");
		jamal_corridor->directions[NORTH] = "jamal_house_block";
		jamal_corridor->directions[EAST] = "jamal_kitchen";
		jamal_corridor->directions[SOUTH] = "jamal_staircase";
		jamal_corridor->directions[WEST] = "jamal_bedroom";

		world->add_child(jamal_corridor);
	}

	{
		Room* jamal_house_block = new Room("jamal_house_block", "As you open the door, an angry mob of thugs accosts you. They drag you to the ground and kill you. You die, killed by the angry mob of thugs.");
		
		world->add_child(jamal_house_block);
	}

	{
		Room* jamal_kitchen = new Room("jamal_kitchen", "Jamal's Kitchen\nThis beautiful, state-of-the-art culinary space would make Martha Stewart turn over in her grave with envy.");
		jamal_kitchen->directions[WEST] = "jamal_corridor";

		Object* sink = new Object("sink", "A large stainless sink glitters like a jewel, inset into a spacious marble countertop.");
		jamal_kitchen->add_child(sink);

		Object* oven = new Object("oven", "This kitchen boasts a sleek stainless-steel oven.");
		jamal_kitchen->add_child(oven);

		Object* cb = new Object("cupboards", "A row of polished oak cupboards are tucked beneath the marble vanity.");
		jamal_kitchen->add_child(cb);

		world->add_child(jamal_kitchen);
	}

	{
		Room* jamal_staircase = new Room("jamal_staircase", "Staircase\nA rickety staircase descending into the basement.");
		jamal_staircase->directions[UP] = "jamal_corridor";
		jamal_staircase->directions[DOWN] = "henrik_library";

		world->add_child(jamal_staircase);
	}

	{
		Room* henrik_library = new Room("henrik_library", "Henrik's Library\nThis subterranean den is where the father of realism does his stuff.");
		henrik_library->directions[NORTH] = "jamal_staircase";
		Object* desk = new Object("desk", "A green banker's lamp dimly lights Henrik's cluttered desk.");
		henrik_library->add_child(desk);
		Object* shelf = new Object("bookshelf", "A massive bookshelf covers the wall to the west.");
		shelf->deep_description = "The shelf is full of books written by dead white men.";
		henrik_library->add_child(shelf);
		Object* book = new Object("book", "One book protrudes farther than the rest.");
		book->properties |= Object::TAKEABLE;
		book->deep_description = "The book is Ghosts, by Henrik Ibsen.";
		shelf->add_child(book);
		Object* secret_switch = new Object("switch", "A secret switch protrudes from the empty slot where a book used to be.");
		secret_switch->properties = Object::HITTABLE;
		secret_switch->run_action = [=](World* w, Terminal* t, Action* a, Object* o)
		{
			if (a->name.id == a->name.parent_list->HIT)
			{
				t->disp("Hitting the switch causes the bookshelf to slide to the side, revealing a doorway leading to the west.");
				henrik_library->shallow_description += "\nA doorway leads to the west.";
				shelf->shallow_description = "A massive bookshelf is slid to one side of the west wall.";
				henrik_library->directions[WEST] = "henrik_lab";
				return true;
			}
			return true;
		};
		henrik_library->add_child(secret_switch);
		book->run_action = [=](World* w, Terminal* t, Action* a, Object* o)
		{
			if (a->name.id == a->name.parent_list->TAKE)
			{
				t->disp("Taking the book reveals a secret switch.");
				shelf->shallow_description += " One book is missing, leaving an empty slot.";
				shelf->deep_description += " One book is missing, leaving an empty slot.";
				secret_switch->properties |= Object::VISIBLE;
				secret_switch->describe(t, false, true);
				return true;
			}
			return true;
		};
		world->add_child(henrik_library);
	}

	{
		Room* henrik_lab = new Room("henrik_lab", "Henrik\'s Lab\nAn underground laboratory where Henrik does his stuff.\nA door leads out to the east.");
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
		Room* sewer = new Room("sewer", "Sewer\nA slimy underground pipe, goin' to the west. Sewage runs thickly around your calves as you wade inna water.");
		sewer->directions[WEST] = "jamal_house_block";
        world->add_child(sewer);
	}

	return world;
}
