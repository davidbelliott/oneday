#include "level_data.h"
#include "Player.h"
#include "Room.h"
#include "World.h"

World* generate_world()
{
	World* world = new World();
	world->flags =
	{
		{ "alive", 1 },
		{ "health", 100 },
		{ "woke_up", 0 }
	};
	world->cur_room = "temp_lane";
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
		/*hole->pre_action = [](World* w, Receiver* r, Action* a, Object* o)
		{
			if (a->name.id == a->name.parent_list->GO)
			{
				if (w->player->clothing == "hazmat")
				{
					r->add_event(std::make_shared<CmdDisp>("With the hazmat suit on, you tentatively step down into the hole and lower yourself into the murky water.\nIt rises gurgling to your neck.\nWith a desperate resignation, you plunge beneath the dark sewage."));
                    r->add_event(std::make_shared<CmdPause>());
                    return true;
				}
				else
				{
					r->add_event(std::make_shared<CmdDisp>("The hole is full of disgusting sewage water. You want to touch it with your bare skin? Kimochi warui~"));
                    return false;
				}
			}
		};*/
		jamal_bathroom->add_child(hole);

		world->add_child(jamal_bathroom);
	}

    {
        Room* temp_lane = new Room("temp_lane", "Temporary Lane", "A temporary lane.");
        temp_lane->directions[NORTH] = "hood_avenue";
        /*temp_lane->post_action = [](World* w, Receiver* r, Action* a, Object* o)
        {
            if(a->name.id == a->name.parent_list->LOOK && w->get_flag["thugfight_outcome"] == 0)
            {
                r->add_event(std::make_shared<CmdDisp>("Suddenly, a group of thugs rounds the corner. They raise fists to attack you!\nPress any key to tense your abs and deflect their blows."));
                r->add_event(std::make_shared<CmdPause>());
                r->add_event(std::make_shared<CmdAddGameState>(new GameStateThugFight()));
                r->add_event(std::make_shared<CmdCustom>( [](GameState* g)
                            {
                                if(g->get_world()->get_flag("thugfight_outcome") == 1)  // Won the fight
                                {
                                    g->get_terminal()->disp("Cowed by your abdominal prowess, the thugs slink off.");
                                }
                                else
                                {
                                    g->get_terminal()->disp("Your abdomen is hard and tender from the repeated blows. You die.");
                                    g->get_world()->set_flag("active", 0);
                                }
                            } );
               r->add_event(std::make_shared<CmdPause>());
            }
            return true;
        };*/
        world->add_child(temp_lane);
    }

	return world;
}
