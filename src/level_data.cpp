#include "level_data.h"
#include "Engine.h"
#include "GameStateIntro.h"
#include "GameStateText.h"
#include "GameStateThugFight.h"
#include "GameStateMenu.h"
#include "Player.h"
#include "World.h"

void execute()
{
    Engine* engine = new Engine();
    GameStateText* text = new GameStateText(engine);

    // Generate the world
	World* world = text->world;

	world->flags =
	{
		{ "alive", 1 },
		{ "health", 100 },
		{ "woke_up", 0 }
	};
	world->cur_room = "compton_street";

    Player* player = new Player("player", "a sturdy creature fond of drink and industry");
    player->pretty_name = "Jamal";
    player->clothing = "";
    world->add_child(player);
    world->set_player(player);
    Object* jamal_bedroom = new Object("jamal_bedroom");
    jamal_bedroom->pretty_name = "Jamal's bedroom";

    ComponentDescription* c_description = new ComponentDescription("This bedroom is a wretched hovel.");
    jamal_bedroom->add_component(c_description);
    jamal_bedroom->add_component(new ComponentRoom(
                {{EAST, "jamal_corridor"},
                {SOUTH, "jamal_bathroom"}}));

    ComponentMusic* c_music = new ComponentMusic("res/good_day.ogg");
    jamal_bedroom->add_component(c_music);
    jamal_bedroom->pre_command = [=](Command* cmd)
    {
        if(cmd->type == Command::LOOK_AROUND)
            text->send_front(std::make_shared<CmdPlayMusic>(c_music->music));
        return true;
    };

    Object* posters = new Object("posters");
    posters->aliases = { "poster", "wall", "walls" };
    posters->add_component(new ComponentDescription("The walls are plastered with grimy old posters.", "The posters feature realistic depictions of mom's spaghetti."));
    jamal_bedroom->add_child(posters);

    Object* window = new Object("window");
    window->add_component(new ComponentDescription("A single smeared window to the north suffuses the room in dim light.", "Looking through the window, you notice a gang of thugs gathered in front of your house."));
    jamal_bedroom->add_child(window);

    Object* paper = new Object("paper");
    paper->aliases = { "sheet" };
    std::string paper_text = "Just waking up in the morning, gotta thank God\nI don't know but today seems kinda odd\nNo barking from the dog, no smog\nAnd momma cooked a breakfast with no hog.";
    paper->add_component(new ComponentDescription("A crumpled sheet of paper lies on the floor."));
    paper->add_component(new ComponentText(paper_text));
    paper->add_component(new ComponentTakeable());
    jamal_bedroom->add_child(paper);

    world->add_child(jamal_bedroom);

    Object* jamal_bathroom = new Object("jamal_bathroom");
    jamal_bathroom->pretty_name = "Jamal's bathroom";
    jamal_bathroom->add_component(c_music);
    jamal_bathroom->add_component(new ComponentDescription("This is where you defecate daily. This cesuo is a reeking pigsty.\nOne door leads to the north."));
    jamal_bathroom->add_component(new ComponentRoom({{NORTH, "jamal_bedroom"}}));

    Object* hole = new Object("hole");
    hole->add_component(new ComponentDescription("A dark hole gapes in the floor, presumably where a toilet used to be."));
    hole->add_component(new ComponentPortal("sewer"));
    hole->pre_command = [=](Command* cmd)
    {
        bool execute = true;
        if(cmd->type == Command::GO)
        {
            if(((Player*)world->get_player())->clothing == "hazmat suit")
            {
                text->send_front(std::make_shared<CmdDisp>("With the hazmat suit on, you tentatively step down into the hole and lower yourself into the murky water.\nIt rises gurgling to your neck.\nWith a desperate resignation, you plunge beneath the dark sewage."));
                text->send_front(std::make_shared<CmdPause>());
                execute = true;
            }
            else
            {
                text->send_front(std::make_shared<CmdDisp>("The hole is full of disgusting sewage water. You want to touch it with your bare skin? Kimochi warui~"));
                execute = false;
            }
        }
        return execute;
    };
            
    jamal_bathroom->add_child(hole);

    world->add_child(jamal_bathroom);

    Object* jamal_corridor = new Object("jamal_corridor");
    jamal_corridor->pretty_name = "the Magick corridor";
    jamal_corridor->add_component(new ComponentDescription("This hallway is imbued with a strong Faerie Magick."));
    jamal_corridor->add_component(new ComponentMusic("res/vulcan.ogg"));
    jamal_corridor->add_component(new ComponentRoom({
                {NORTH, "jamal_house_block"},
                {EAST, "jamal_staircase"},
                {WEST, "jamal_bedroom"},
                {SOUTH, "jamal_kitchen"},
                }));
    world->add_child(jamal_corridor);

    Object* kitchen = new Object("jamal_kitchen");
    kitchen->pretty_name = "the kitchen";
    kitchen->add_component(new ComponentDescription("This rickety cookery would make Martha Stewart turn over in her grave."));
    kitchen->add_component(new ComponentRoom({{NORTH, "jamal_corridor"}}));
    world->add_child(kitchen);

    Object* stove = new Object("stove");
    stove->add_component(new ComponentDescription("An electric stove leans against one wall."));
    kitchen->add_child(stove);

    Object* pan = new Object("pan");
    pan->add_component(new ComponentDescription("There's an aluminum pan on one burner."));
    pan->add_component(new ComponentTakeable());
    stove->add_child(pan);

    Object* pellets = new Object("pellets");
    pellets->add_component(new ComponentDescription("Several brown pellets are in the pan. They do not contain pork."));
    pellets->add_component(new ComponentTakeable());
    pan->add_child(pellets);

    Object* staircase = new Object("jamal_staircase");
    staircase->pretty_name = "the staircase";
    staircase->add_component(new ComponentDescription("A rickety staircase descending into the basement."));
    staircase->add_component(new ComponentRoom({
                {UP, "jamal_corridor"},
                {DOWN, "library"}
                }));
    world->add_child(staircase);

    Object* library = new Object("library");
    library->pretty_name = "Thomas Pynchon's library";
    library->add_component(new ComponentDescription("This subterranean den is where the father of realism does his stuff.\nHe is beginning to feel like a rap god."));
    ComponentRoom* c_room = new ComponentRoom({{NORTH, "jamal_staircase"}});
    library->add_component(c_room);

    Object* shelf = new Object("bookshelf");
    shelf->aliases = { "shelf" };
    ComponentDescription* c_desc = new ComponentDescription("A massive bookshelf covers the wall to the west.", "The shelf is full of books written by dead white men.");
    shelf->add_component(c_desc);
    library->add_child(shelf);

    Object* secret_switch = new Object("switch");
    secret_switch->active = false;
    secret_switch->add_component(new ComponentDescription("A secret switch is snug in the cuddly nook where a book used to be."));
    ComponentHittable* c_hit = new ComponentHittable();
    secret_switch->add_component(c_hit);
    secret_switch->pre_command = [=](Command* cmd) {
        if(cmd->type == Command::HIT && c_hit->flipped)
        {
            text->send_front(std::make_shared<CmdDisp>("Hitting the switch again has no effect."));
            return false;
        }
        return true;
    };
    secret_switch->post_command = [=](Command* cmd) {
        if(cmd->type == Command::HIT)
        {
                text->send_front(std::make_shared<CmdDisp>("Hitting the switch causes the bookshelf to slide to the side, revealing a doorway leading to the west."));
                c_desc->initial_appearance = "A massive bookshelf is slid to one side of the west wall.";
                c_room->directions[WEST] = "lab";
        }
    };
    shelf->add_child(secret_switch);

    Object* book = new Object("book");
    book->add_component(new ComponentDescription("One book protrudes farther than the rest."));
    book->add_component(new ComponentTakeable());
    book->post_command = [=](Command* cmd)
    {
        if(cmd->type == Command::TAKE)
        {
            text->send_front(std::make_shared<CmdDisp>("Taking the book reveals a secret switch."));
            c_desc->initial_appearance += " One book is missing, leaving an empty slot.";
            c_desc->description += " One book is missing, leaving an empty slot.";
            secret_switch->active = true;
            secret_switch->discovered = true;
        }
    };
    shelf->add_child(book);
    
    world->add_child(library);

    Object* lab = new Object("lab");
    lab->pretty_name = "Thomas Pynchon's lab";
    lab->add_component(new ComponentDescription("This is the underground lab where Pynchon does his stuff."));
    lab->add_component(new ComponentRoom({{EAST, "library"}}));
    lab->add_component(new ComponentMusic("res/100kilos.ogg"));
    world->add_child(lab);

    Object* table = new Object("table");
    table->add_component(new ComponentDescription("A polished granite table sits in the middle of the room."));
    lab->add_child(table);

    Object* instruments = new Object("instruments");
    instruments->add_component(new ComponentDescription("The table supports several scientific instruments.", "You'd play the instruments if you knew how."));
    table->add_child(instruments);

    Object* lockers = new Object("lockers");
    lockers->add_component(new ComponentDescription("Three metal lockers line the wall."));
    lab->add_child(lockers);

    Object* hazmat = new Object("hazmat suit");
    hazmat->aliases = {"hazmat", "suit"};
    hazmat->add_component(new ComponentDescription("A hazmat suit is hanging inside one locker."));
    hazmat->add_component(new ComponentTakeable());
    hazmat->add_component(new ComponentWearable());
    lockers->add_child(hazmat);

    Object* sewer = new Object("sewer");
    sewer->pretty_name = "a sewer tunnel";
    sewer->add_component(new ComponentRoom({{WEST, "sewer_west"},{EAST, "shaft"}}));
    sewer->add_component(new ComponentDescription("Filthy water flows around your ankles."));
    world->add_child(sewer);

    Object* sewer_west = new Object("sewer_west");
    sewer_west->pretty_name = "a sewer tunnel";
    sewer_west->add_component(new ComponentRoom({{EAST, "sewer"}, {WEST, "sewer_deadend"}}));
    sewer_west->add_component(new ComponentDescription("A vertical tunnel joins the sewer from above."));
    world->add_child(sewer_west);

    Object* ladder = new Object("ladder");
    ladder->add_component(new ComponentClimbable({{UP, "sewer_upper"}}));
    ladder->add_component(new ComponentDescription("A metal ladder is mounted to the concrete wall of the vertical tunnel."));
    sewer_west->add_child(ladder);

    Object* sewer_upper = new Object("sewer_upper");
    sewer_upper->pretty_name = "a sewer access manhole";
    sewer_upper->add_component(new ComponentDescription("This is a vertical tunnel about a meter in diameter."));
    ComponentRoom* sewer_upper_c_room = new ComponentRoom({{DOWN, "sewer_west"}});
    sewer_upper->add_component(sewer_upper_c_room);
    world->add_child(sewer_upper);

    Object* ladder0 = new Object("ladder");
    ladder0->add_component(new ComponentDescription("A metal ladder is mounted to the concrete wall of the vertical tunnel."));
    sewer_upper->add_child(ladder0);

    Object* del_mar = new Object("del_mar");

    Object* manhole_cover = new Object("manhole cover");
    manhole_cover->aliases = {"manhole", "cover", "lid"};
    manhole_cover->add_component(new ComponentMoveable());
    manhole_cover->add_component(new ComponentDescription("A heavy manhole cover blocks your exit above, but it looks like you could move it."));
    manhole_cover->post_command = [=](Command* cmd) {
        if(cmd->type == Command::MOVE)
        {
            sewer_upper_c_room->directions[UP] = "del_mar";
            sewer_upper->remove_child(manhole_cover);
            del_mar->add_child(manhole_cover);
        }
    };
    sewer_upper->add_child(manhole_cover);

    Object* sewer_deadend = new Object("sewer_deadend");
    sewer_deadend->pretty_name = "a sewer tunnel";
    sewer_deadend->add_component(new ComponentRoom({{EAST, "sewer_west"}}));
    world->add_child(sewer_deadend);

    Object* grate = new Object("grating");
    grate->add_component(new ComponentDescription("The water here flows through a large grating, which blocks your way."));
    sewer_deadend->add_child(grate);

    del_mar->pretty_name = "Del Mar Boulevard";
    del_mar->add_component(new ComponentRoom({{NORTH, "compton_street"}}));
    del_mar->add_component(new ComponentDescription("A temporary lane."));
    del_mar->pre_command = [=](Command* cmd)
    {
        if(cmd->type == Command::LOOK_AROUND && world->get_flag("thug_fight_outcome") == 0)
        {
            text->send_front(std::make_shared<CmdDisp>("Suddenly, a group of thugs rounds the corner. They raise fists to attack you!"));
            text->send_front(std::make_shared<CmdPause>());
            text->send_front(std::make_shared<CmdDisp>("Press A, S, D, or F in rhythm with the punches to deflect their blows."));
            text->send_front(std::make_shared<CmdPause>());
            text->send_front(std::make_shared<CmdAddGameState>(new GameStateThugFight(engine)));
            world->set_flag("thug_fight_outcome", 1);
        }
        return true;
    };
    world->add_child(del_mar);

    Object* manhole = new Object("manhole");
    manhole->add_component(new ComponentDescription("A manhole gapes open in the middle of the street."));
    manhole->add_component(new ComponentPortal("sewer_upper"));
    del_mar->add_child(manhole);

    Object* compton_street = new Object("compton_street");
    compton_street->pretty_name = "Compton Street";
    compton_street->add_component(new ComponentRoom({{NORTH, "lil_wayne_front"}, {EAST, "magdalene_lane"}, {SOUTH, "del_mar"}, {WEST, "vacant_lot"}}));
    compton_street->add_component(new ComponentDescription("This strip of gritty asphalt comes straight outta the dark and unknown reaches of the city of Compton."));
    world->add_child(compton_street);

    int n_talks = 0;
    Object* urban_youth = new Object("urban youth");
    urban_youth->aliases = {"youth", "man", "person", "guy", "dude", "urban", "stranger", "him"};
    urban_youth->add_component(new ComponentDescription("An urban youth stands in the corner, his hands tucked inside hoodie pockets."));
    ComponentTalkable* youth_c_talkable = new ComponentTalkable({"Why are you standing here?",
                "-I'm here for the people, Jamal.",
                "W...what do you mean?",
                "-The man has kept us down for too long, Jamal.",
                "-You must report our grievances 2 the president.",
                "But how?",
                "-Viper will host a rap contest in the Club. The winner goes to the White House.",
                "-You must challenge him, and emerge the victor."});
    urban_youth->add_component(youth_c_talkable);
    urban_youth->post_command = [&](Command* cmd) {
        std::string output = "The youth looks at you ";
        if(cmd->type == Command::TALK_TO)
        {
            n_talks = -1;
            youth_c_talkable->talkable_data = {"-Ain't you done yet?"};
        }
        else
        {
            if(n_talks <= 3)
            {
                if(n_talks == -1)
                    output += "encouragingly";
                else if(n_talks == 0)
                    output += "impassively";
                else if(n_talks == 1)
                    output += "quizzically";
                else if(n_talks == 2)
                    output += "indignantly";
                else if(n_talks == 3)
                    output += "furiously";
                output += ".";
                text->send_front(std::make_shared<CmdDisp>(output));
            }
            else
            {
                text->send_front(std::make_shared<CmdDisp>("The youth strikes out and delivers a fatal kick to your pancreas.\nYou die."));
                text->send_front(std::make_shared<CmdPause>());
                text->send_front(std::make_shared<CmdQuit>());
            }
            if(n_talks >= 0)
                n_talks++;
        }
    };
    compton_street->add_child(urban_youth);
    
    Object* vacant_lot = new Object("vacant_lot");
    vacant_lot->pretty_name = "a Vacant Lot";
    vacant_lot->add_component(new ComponentRoom({{EAST, "compton_street"}}));
    vacant_lot->add_component(new ComponentDescription("This lot is empty."));
    world->add_child(vacant_lot);

    Object* grass = new Object("grass");
    grass->add_component(new ComponentDescription("The politics of bacteria, the soil's stringing of rings and chains in nets only God can tell the meshes of, have seen the grass thrive often to lengths of a foot and a half, yes amazing but true."));
    vacant_lot->add_child(grass);

    Object* magnet = new Object("magnet");
    magnet->add_component(new ComponentTakeable());
    magnet->add_component(new ComponentDescription("A magnet lies in the grass.", "Who can fathom the mysterious workings of the magnet?"));
    grass->add_child(magnet);

    Object* magdalene_lane = new Object("magdalene_lane");
    magdalene_lane->pretty_name = "Magdalene Lane";
    magdalene_lane->add_component(new ComponentRoom({{WEST, "compton_street"}, {NORTH, "garbage_alley"}, {EAST, "kolob_street"}}));
    magdalene_lane->add_component(new ComponentDescription("The red-light domain where everybody's soul is for sale."));
    world->add_child(magdalene_lane);

    Object* garbage_alley = new Object("garbage_alley");
    garbage_alley->pretty_name = "a dim alley overflowing with garbage";
    garbage_alley->add_component(new ComponentRoom(
                {{SOUTH, "magdalene_lane"}})),
    world->add_child(garbage_alley);

    Object* garbage_cans = new Object("garbage cans");
    garbage_cans->aliases = {"cans", "garbage", "rubbish", "bins", "can", "bin"};
    garbage_cans->add_component(new ComponentDescription("Several cans huddle by the wall in a pool of sodium-vapor light."));
    garbage_cans->add_component(new ComponentClimbable({{UP, "can tops"}}));
    garbage_alley->add_child(garbage_cans);

    Object* can_tops = new Object("can tops");
    can_tops->pretty_name = "the top of the garbage cans";
    can_tops->add_component(new ComponentDescription("They wobble precariously beneath your feet."));
    can_tops->add_component(new ComponentRoom(
                {{DOWN, "garbage_alley"}}));
    world->add_child(can_tops);

    Object* drain_pipe = new Object("drain pipe");
    drain_pipe->aliases = {"pipe", "drain"};
    drain_pipe->add_component(new ComponentDescription("A drain pipe runs down the wall on one side."));
    drain_pipe->add_component(new ComponentClimbable({{UP, "roof"}}));
    can_tops->add_child(drain_pipe);

    Object* roof = new Object("roof");
    roof->pretty_name = "the roof";
    roof->add_component(new ComponentRoom({{DOWN, "can tops"}}));
    roof->add_component(new ComponentDescription("You hear the regular dripping of water. The roof is dark and tarry."));
    world->add_child(roof);

    Object* rope = new Object("rope");
    rope->add_component(new ComponentTakeable());
    rope->add_component(new ComponentDescription("A coil of rope lies to one side."));
    roof->add_child(rope);

    Object* kolob_street = new Object("kolob_street");
    kolob_street->pretty_name = "Kolob Street";
    kolob_street->add_component(new ComponentRoom({{SOUTH, "jamal_corridor"}, {WEST, "magdalene_lane"}}));
    world->add_child(kolob_street);
    
    Object* shaft = new Object("shaft");
    shaft->pretty_name = "a ledge overlooking a deep shaft";
    shaft->add_component(new ComponentRoom({{WEST, "sewer"}}));
    shaft->add_component(new ComponentDescription("You see sunlight filtering down from a grate high above your head.\nRumbling and clanking sounds drift up from the darkness below."));
    world->add_child(shaft);
    
    /*cmd_ptr describe = std::make_shared<CmdDescribe>();
    describe->add_object((Object*)engine->world->get_current_room());
    text->send(describe);*/
    //world->get_current_room()->describe(text);
    text->send(std::make_shared<CmdInput>());
    engine->push_state(text);

    sf::Clock clock;
    sf::Time dt;
    while(engine->running)
    {
        // Collect input from the user
        engine->get_input();

        // Let gamestates handle their pending events
        engine->execute_commands();

        // Update gamestates based on elapsed time
        dt = clock.restart();
        engine->update(dt);

        // Draw gamestates
        engine->draw();

        engine->prune();
        // Sleep for remaining time
        while(clock.getElapsedTime().asSeconds() < 1.0f / config::update_frequency)
        {
            sf::sleep(sf::milliseconds(1.0f));
        }
    }

    delete text;
	delete engine;
}
