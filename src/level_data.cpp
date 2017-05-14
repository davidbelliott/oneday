#include "level_data.h"
#include "Engine.h"
#include "GameStateText.h"
#include "GameStateThugFight.h"
//#include "GameStateSubway.h"
//#include "GameStateMenu.h"
#include "Player.h"
#include "World.h"
#include "Terminal.h"
#include <iostream>

bool jamal_bedroom_before(Command* cmd, GameState* g) {
    g->engine->audio->play_music("res/good_day.ogg");
    return true;
}

bool thugs_before(Command* cmd, GameState* g) {
    g->engine->terminal->disp("The thugs are too far away.");
    return false;
}

bool sewer_before(Command* cmd, GameState* g) {
    bool execute = true;
    if(cmd->type == Command::GO && g->world->cur_room == "jamal_bathroom")
    {
        if(((Player*)g->world->get_player())->clothing == "hazmat suit")
        {
            g->engine->terminal->disp("With the hazmat suit on, you tentatively step down into the hole and lower yourself into the murky water.\nIt rises gurgling to your neck.\nWith a desperate resignation, you plunge beneath the dark sewage.");
            g->engine->terminal->pause();
            execute = true;
        }
        else
        {
            g->engine->terminal->disp("The hole is full of disgusting sewage water. You want to touch it with your bare skin? Kimochi warui~");
            execute = false;
        }
    }
    return execute;
}

bool jamal_front_before(Command* cmd, GameState* g) {
    if(cmd->type == Command::GO && g->world->get_flag("thug_fight_outcome") == 0)
    {
        g->engine->terminal->disp("You hear the intense rustling of thugs outside your door. You'd best not go out this way.");
        return false;
    }
    return true;
}

bool tasted_pellets = false;
bool pellets_before(Command* cmd, GameState* g) {
    if(cmd->type == Command::EAT)
    {
        if(tasted_pellets)
            g->engine->terminal->disp("You don't want to eat any more.");
        else
        {
            g->engine->terminal->disp("You tentatively nibble at one of the mysterious pellets. It contains crack and weed. The combination makes your eyes bleed.");
            tasted_pellets = true;
        }
        return false;
    }
    return true;
}

bool switch_flipped = false;
bool secret_switch_before(Command* cmd, GameState* g) {
    if(cmd->type == Command::HIT && switch_flipped)
    {
        g->engine->terminal->disp("Hitting the switch again has no effect.");
        return false;
    }
    return true;
}

ComponentDescription* c_desc = nullptr;
ComponentRoom* c_room = nullptr;
void secret_switch_after(Command* cmd, GameState* g) {
    if(cmd->type == Command::HIT)
    {
            g->engine->terminal->disp("Hitting the switch causes the bookshelf to slide to the side, revealing a doorway leading to the west.");
            c_desc->initial_appearance = "A massive bookshelf is slid to one side of the west wall.";
            c_room->directions[WEST] = "lab";
            switch_flipped = true;
    }
}

Object* secret_switch = nullptr;
void book_after(Command* cmd, GameState* g) {
    if(cmd->type == Command::TAKE)
    {
        g->engine->terminal->disp("Taking the book reveals a secret switch.");
        c_desc->initial_appearance += " One book is missing, leaving an empty slot.";
        c_desc->description += " One book is missing, leaving an empty slot.";
        secret_switch->active = true;
        secret_switch->discovered = true;
    }
}

void hazmat_after(Command* cmd, GameState* g) {
    if(cmd->type == Command::WEAR)
        g->engine->terminal->disp("The suit is rough and tough like leather.");
}

Object* sewer_upper = NULL;
Object* del_mar = NULL;
Object* manhole_cover = NULL;
ComponentRoom* sewer_upper_c_room = NULL;
bool manhole_moved = false;
bool manhole_cover_before(Command* cmd, GameState* g) {
    if(cmd->type == Command::MOVE)
    {
        if(!manhole_moved)
        {
            sewer_upper_c_room->directions[UP] = "del_mar";
            sewer_upper->remove_child(manhole_cover);
            del_mar->add_child(manhole_cover);
            manhole_moved = true;
            return true;
        }
        else
        {
            g->engine->terminal->disp("Why would you want to move the manhole cover back?");
            return false;
        }
    }
    return true;
}

void manhole_cover_after(Command* cmd, GameState* g) {
    if(cmd->type == Command::MOVE)
    {
    }
}

ComponentTalkable* youth_c_talkable = NULL;
int n_talks = 0;
void youth_after(Command* cmd, GameState* g) {
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
            g->engine->terminal->disp(output);
        }
        else
        {
            g->engine->terminal->disp("The youth strikes out and delivers a fatal kick to your pancreas.\nYou die.");
            g->engine->terminal->pause();
            g->engine->running = false;
        }
        if(n_talks >= 0)
            n_talks++;
    }
}

enum LilWayneStatus {
    SEIZURE,
    ANGRY,
    DEAD
} lil_wayne_status = SEIZURE;

bool lil_wayne_inside_before(Command* cmd, GameState* g) {
    if(cmd->type == Command::GO)
    {
        if(lil_wayne_status == ANGRY)
        {
            if(g->world->player->has_direct_child("hedge clippers"))
            {
                g->engine->terminal->disp("Hedge clippers in hand, you creep stealthily into the domain of Birdman Jr.");
            }
            else
            {
                g->engine->terminal->disp("Lil Wayne is bretty mad :-DD best not go in here unarmed.");
                return false;
            }
        }
    }
    return true;
}

void lil_wayne_front_after(Command* cmd, GameState* g) {
    if(cmd->type == Command::LOOK_AROUND)
    {
        if(lil_wayne_status == SEIZURE)
            g->engine->terminal->disp("You hear loud, convulsive fits from within the house. Mr. Crazy Flow could be in trouble!");
        else if(lil_wayne_status == ANGRY)
        {
                int choice = rand() % 5;
                std::string output = "Lil Wayne shouts at you from the window:\n(Lil Wayne) Get the fuck out, you ";
                if(choice == 0)
                    output += "varlot";
                else if(choice == 1)
                    output += "rank tickle-brained fustilarian";
                else if(choice == 2)
                    output += "bitch ass nigga";
                else if(choice == 3)
                    output += "lewdster";
                else if(choice == 4)
                    output += "baka gaijin";
                g->engine->terminal->disp(output);
        }
    }
}

int lil_wayne_interaction_counter = 0;
void lil_wayne_after(Command* cmd, GameState* g) {
    if(lil_wayne_status == SEIZURE)
    {
        if(cmd->type == Command::GIVE && static_cast<CmdGive*>(cmd)->objects[1]->name == "pellets")
        {
            lil_wayne_status = ANGRY;
            g->engine->terminal->disp("Lil Wayne recovers from his seizure!\nIt's A Christmas miracle.");
            g->engine->terminal->pause();
            g->engine->terminal->disp("Yet as he clambers up from the ground, he suddenly turns hostile:");
            g->engine->terminal->disp("-ayy wat u doin in my house boi");
            g->engine->terminal->pause();
            g->engine->terminal->disp("I just saved your ass from a seizure");
            g->engine->terminal->pause();
            g->engine->terminal->disp("-nigga u better scram fore ah goes flop-bott on u");
            //text->send_front(talk);
            g->engine->terminal->disp("Lil Wayne lunges for you, and you barely escape out the door.");
            g->engine->terminal->pause();
            CmdGo go;
            go.new_room = "lil_wayne_front";
            go.run_with_callbacks(g);
        }
        else
        {
            if(lil_wayne_interaction_counter % 2 == 0)
                g->engine->terminal->disp("Lil Wayne writhes on the ground like a monsta.");
            else
                g->engine->terminal->disp("Perhaps if you feed medicine to Lil Wayne he'll get better.");
            lil_wayne_interaction_counter++;
        }
    }
}

bool can_jump = false;
bool fire_escape_before(Command* cmd, GameState* g) {
    if(cmd->type == Command::GO && ((CmdGo*)cmd)->new_room == "kolob_street_east")
    {
        if(can_jump)
        {
            g->engine->terminal->disp("You take a flying leap from the fire escape and land directly atop the soft body pillow, which breaks your fall.");
            return true;
        }
        g->engine->terminal->disp("Jumping from this high could be fatal. Maybe somethin' soft would break the fall.");
        return false;
    }
    return true;
}

Object* body_pillow = nullptr;
Object* kolob_street_east = nullptr;
void fire_escape_after(Command* cmd, GameState* g) {
    if(cmd->type == Command::THROW && ((CmdThrow*)cmd)->objects[1] == body_pillow)
    {
        g->engine->terminal->disp("The body pillow lands on the street below.");
        body_pillow->parent->remove_child(body_pillow);
        kolob_street_east->add_child(body_pillow);
        can_jump = true;
    }
}

void del_mar_after(Command* cmd, GameState* g) {
    if(cmd->type == Command::GO && g->world->get_flag("thug_fight_outcome") == 0)
    {
        g->engine->terminal->disp("Suddenly, a group of thugs rounds the corner. They raise fists to attack you!");
        g->engine->terminal->pause();
        g->engine->terminal->disp("Each time a fist hits you, hit A, S, D, or F to tense the corresponding ab and deflect the blow.");
        g->engine->terminal->disp("Undeflected blows will push your abs back until you perish.");
        g->engine->terminal->pause();
        g->engine->push_state(new GameStateThugFight(g->engine));
        g->world->set_flag("thug_fight_outcome", 1);
    }
}

World* generate_world(Engine* engine)
{
    // Generate the world
    World* world = new World();

	world->flags =
	{
		{ "alive", 1 },
		{ "health", 100 },
		{ "woke_up", 0 }
	};
	world->cur_room = "sewer_upper";

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
                {NORTH, "jamal_bathroom"}}));

    ComponentMusic* c_music = new ComponentMusic("res/good_day.ogg");
    jamal_bedroom->add_component(c_music);
    jamal_bedroom->before = &jamal_bedroom_before;

    Object* posters = new Object("posters");
    posters->aliases = { "poster", "wall", "walls" };
    posters->add_component(new ComponentDescription("The walls are plastered with grimy old posters.", "The posters feature realistic depictions of mom's spaghetti."));
    jamal_bedroom->add_child(posters);

    Object* window = new Object("window");
    window->add_component(new ComponentDescription("A single smeared window to the north suffuses the room in dim light."));
    jamal_bedroom->add_child(window);

    Object* thugs = new Object("thugs");
    thugs->aliases = {"thug"};
    thugs->add_component(new ComponentDescription("You notice a gang of thugs gathered in front of your house."));
    thugs->before = &thugs_before;
    
    window->add_child(thugs);

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
    ComponentMusic* music_hard = new ComponentMusic("res/hypnotize.ogg");
    jamal_bathroom->add_component(music_hard);
    jamal_bathroom->add_component(new ComponentDescription("This is where you defecate daily. This 廁所 is a reeking pigsty."));
    jamal_bathroom->add_component(new ComponentRoom({{SOUTH, "jamal_bedroom"}}));

    Object* hole = new Object("hole");
    hole->add_component(new ComponentDescription("In the floor a hole is gaping, presumably where a toilet used to be.", "The hole looks big enough for a nigga."));
    hole->add_component(new ComponentPortal("sewer"));
            
    jamal_bathroom->add_child(hole);

    world->add_child(jamal_bathroom);

    Object* jamal_corridor = new Object("jamal_corridor");
    jamal_corridor->pretty_name = "the Magick corridor";
    jamal_corridor->add_component(new ComponentDescription("This hallway is imbued with a strong Faerie Magick."));
    jamal_corridor->add_component(new ComponentMusic("res/vulcan.ogg"));
    jamal_corridor->add_component(new ComponentRoom({
                {NORTH, "jamal_front"},
                {EAST, "jamal_staircase"},
                {WEST, "jamal_bedroom"},
                {SOUTH, "jamal_kitchen"},
                }));

    world->add_child(jamal_corridor);

    Object* kitchen = new Object("jamal_kitchen");
    kitchen->pretty_name = "the kitchen";
    kitchen->add_component(new ComponentDescription("This rickety cookery would make Martha Stewart turn over in her grave."));
    kitchen->add_component(new ComponentRoom({{NORTH, "jamal_corridor"}}));
    kitchen->add_component(new ComponentMusic("res/vulcan.ogg"));
    world->add_child(kitchen);

    Object* stove = new Object("stove");
    stove->add_component(new ComponentDescription("An electric stove leans against one wall."));
    kitchen->add_child(stove);

    Object* jar = new Object("jar");
    jar->add_component(new ComponentDescription("There's a clear glass jar on top of the stove."));
    jar->add_component(new ComponentTakeable());
    jar->add_component(new ComponentOpenClose());
    stove->add_child(jar);

    Object* pellets = new Object("pellets");
    pellets->aliases = {"pellet", "medicine", "pill", "crack", "weed"};
    pellets->add_component(new ComponentDescription("Several brown pellets are in the jar. They do not contain pork."));
    pellets->add_component(new ComponentTakeable());
    pellets->add_component(new ComponentEdible());
    pellets->before = &pellets_before;
    pellets->discovered = true;
    jar->add_child(pellets);

    Object* staircase = new Object("jamal_staircase");
    staircase->pretty_name = "the staircase";
    staircase->add_component(new ComponentDescription("A rickety staircase descending into the basement."));
    staircase->add_component(new ComponentRoom({
                {UP, "jamal_corridor"},
                {DOWN, "library"}
                }));
    staircase->add_component(new ComponentMusic("res/dope.ogg"));
    world->add_child(staircase);

    Object* library = new Object("library");
    library->pretty_name = "Thomas Pynchon's library";
    library->add_component(new ComponentDescription("This subterranean den is where the father of realism does his stuff.\nHe is beginning to feel like a rap god."));
    c_room = new ComponentRoom({{NORTH, "jamal_staircase"}});
    library->add_component(c_room);
    library->add_component(new ComponentMusic("res/dope.ogg"));

    Object* shelf = new Object("bookshelf");
    shelf->aliases = { "shelf" };
    c_desc = new ComponentDescription("A massive bookshelf covers the wall to the west.", "The shelf is full of books written by dead white men.");
    shelf->add_component(c_desc);
    library->add_child(shelf);

    secret_switch = new Object("switch");
    secret_switch->active = false;
    secret_switch->add_component(new ComponentDescription("A secret switch is snug in the cuddly nook where a book used to be."));
    ComponentHittable* c_hit = new ComponentHittable();
    secret_switch->add_component(c_hit);
    secret_switch->before = &secret_switch_before;
    secret_switch->after = &secret_switch_after;
    shelf->add_child(secret_switch);

    Object* book = new Object("book");
    book->add_component(new ComponentDescription("One book protrudes farther than the rest."));
    book->add_component(new ComponentTakeable());
    book->add_component(new ComponentText("This page intentionally left blank."));
    book->after = &book_after;
    shelf->add_child(book);
    
    world->add_child(library);

    Object* lab = new Object("lab");
    lab->pretty_name = "Thomas Pynchon's lab";
    lab->add_component(new ComponentDescription("This is the underground lab where Pynchon does his stuff."));
    lab->add_component(new ComponentRoom({{EAST, "library"}}));
    lab->add_component(new ComponentMusic("res/dope.ogg"));
    world->add_child(lab);

    Object* table = new Object("table");
    table->add_component(new ComponentDescription("A polished granite table sits in the middle of the room."));
    lab->add_child(table);

    Object* instruments = new Object("instruments");
    instruments->add_component(new ComponentDescription("The table supports several scientific instruments.", "You'd play the instruments if you knew how."));
    table->add_child(instruments);

    Object* lockers = new Object("lockers");
    lockers->aliases = {"locker"};
    lockers->add_component(new ComponentDescription("Three metal lockers line the wall."));
    lockers->add_component(new ComponentOpenClose(false));
    lab->add_child(lockers);

    Object* hazmat = new Object("hazmat suit");
    hazmat->aliases = {"hazmat", "suit"};
    hazmat->add_component(new ComponentDescription("A hazmat suit is hanging inside one locker.", "The hazmat suit is rough and tough like leather."));
    hazmat->add_component(new ComponentTakeable());
    hazmat->add_component(new ComponentWearable());
    hazmat->after = hazmat_after;
    lockers->add_child(hazmat);


    Object* sewer = new Object("sewer");
    sewer->pretty_name = "a sewer tunnel";
    sewer->add_component(music_hard);
    sewer->add_component(new ComponentRoom({{WEST, "sewer_west"},{EAST, "shaft"}}));
    sewer->add_component(new ComponentDescription("Filthy water flows around your ankles."));
    sewer->before = sewer_before;
    world->add_child(sewer);

    Object* sewer_west = new Object("sewer_west");
    sewer_west->pretty_name = "a sewer tunnel";
    sewer_west->add_component(music_hard);
    sewer_west->add_component(new ComponentRoom({{EAST, "sewer"}, {WEST, "sewer_deadend"}, {UP, "sewer_upper"}}));
    sewer_west->add_component(new ComponentDescription("A vertical tunnel joins the sewer from above."));
    world->add_child(sewer_west);

    Object* ladder = new Object("ladder");
    ladder->add_component(new ComponentDescription("A metal ladder is mounted to the concrete wall of the vertical tunnel."));
    sewer_west->add_child(ladder);

    sewer_upper = new Object("sewer_upper");
    sewer_upper->pretty_name = "a sewer access manhole";
    sewer_upper->add_component(new ComponentDescription("This is a vertical tunnel about a meter in diameter."));
    sewer_upper_c_room = new ComponentRoom({{DOWN, "sewer_west"}});
    sewer_upper->add_component(sewer_upper_c_room);
    world->add_child(sewer_upper);

    Object* ladder0 = new Object("ladder");
    ladder0->add_component(new ComponentDescription("A metal ladder is mounted to the concrete wall of the vertical tunnel."));
    sewer_upper->add_child(ladder0);

    del_mar = new Object("del_mar");

    manhole_cover = new Object("manhole cover");
    manhole_cover->aliases = {"manhole", "cover", "lid"};
    manhole_cover->add_component(new ComponentMoveable());
    manhole_cover->add_component(new ComponentDescription("A heavy manhole cover blocks your exit above, but it looks like you could move it."));
    manhole_cover->before = manhole_cover_before;
    sewer_upper->add_child(manhole_cover);

    Object* sewer_deadend = new Object("sewer_deadend");
    sewer_deadend->pretty_name = "a sewer tunnel";
    sewer_deadend->add_component(music_hard);
    sewer_deadend->add_component(new ComponentRoom({{EAST, "sewer_west"}}));
    world->add_child(sewer_deadend);

    Object* grating = new Object("grating");
    grating->add_component(new ComponentDescription("The water here flows through a large grating, which blocks your way."));
    sewer_deadend->add_child(grating);

    del_mar->pretty_name = "Del Mar Boulevard";
    del_mar->add_component(new ComponentRoom({{NORTH, "compton_street"}}));
    del_mar->add_component(new ComponentDescription("A temporary lane."));
    del_mar->after = del_mar_after;
    world->add_child(del_mar);

    Object* manhole = new Object("manhole");
    manhole->add_component(new ComponentDescription("A manhole gapes open in the middle of the street."));
    manhole->add_component(new ComponentPortal("sewer_upper"));
    del_mar->add_child(manhole);

    Object* compton_street = new Object("compton_street");
    compton_street->pretty_name = "Compton Street";
    compton_street->add_component(new ComponentRoom({{NORTH, "compton_street_north"}, {EAST, "magdalene_lane"}, {SOUTH, "del_mar"}, {WEST, "club_front"}}));
    compton_street->add_component(new ComponentDescription("This strip of gritty asphalt comes straight outta the dark and unknown reaches of Compton."));
    ComponentMusic* music_lucini = new ComponentMusic("res/lucini.ogg");
    compton_street->add_component(music_lucini);
    world->add_child(compton_street);

    Object* urban_youth = new Object("urban youth");
    urban_youth->aliases = {"youth", "man", "person", "guy", "dude", "urban", "stranger", "him"};
    urban_youth->add_component(new ComponentDescription("An urban youth stands in the corner, his hands tucked inside hoodie pockets."));
    youth_c_talkable = new ComponentTalkable({"Why are you standing here?",
                "-I'm here for the people, Jamal.",
                "W...what do you mean?",
                "-The man has kept us down for too long, Jamal.",
                "-You must report our grievances 2 the president.",
                "But how?",
                "-Viper will host a rap contest in the Club. The winner goes to the White House.",
                "-You must challenge him, and emerge the victor."});
    urban_youth->add_component(youth_c_talkable);
    urban_youth->after = youth_after;
    compton_street->add_child(urban_youth);

    Object* compton_street_north = new Object("compton_street_north");
    compton_street_north->pretty_name = "North Compton Street";
    compton_street_north->add_component(new ComponentRoom({{NORTH, "lil_wayne_front"}, {SOUTH, "compton_street"}, {WEST, "vacant_lot"}}));
    compton_street_north->add_component(new ComponentDescription("This road runs from north to south."));
    compton_street_north->add_component(music_lucini);
    world->add_child(compton_street_north);


    Object* lil_wayne_front = new Object("lil_wayne_front");
    lil_wayne_front->pretty_name = "the front of Lil Wayne's home";
    lil_wayne_front->add_component(new ComponentDescription("You face the edifice within which young Tunechi dwells."));
    lil_wayne_front->add_component(new ComponentRoom({{NORTH, "lil_wayne_inside"}, {SOUTH, "compton_street_north"}}));
    lil_wayne_front->add_component(new ComponentMusic("res/6foot.ogg"));
    lil_wayne_front->after = &lil_wayne_front_after;
    world->add_child(lil_wayne_front);


    Object* lil_wayne_inside = new Object("lil_wayne_inside");
    lil_wayne_inside->pretty_name = "the inside of Lil Wayne's humble abode";
    lil_wayne_inside->add_component(new ComponentRoom({{SOUTH, "lil_wayne_front"}}));
    lil_wayne_inside->add_component(new ComponentDescription("It bears a vague resemblance to a wigwam."));
    lil_wayne_inside->add_component(new ComponentMusic("res/6foot.ogg"));
    lil_wayne_inside->before = lil_wayne_inside_before;
    world->add_child(lil_wayne_inside);

    Object* lil_wayne = new Object("Lil Wayne");
    lil_wayne->aliases = {"lil", "wayne", "him", "he" };
    lil_wayne->add_component(new ComponentDescription("Lil Wayne is having a seizure! His muscles are rigid, and he experiences brief losses of consciousness."));
    lil_wayne->add_component(new ComponentTalkable({"-hnnng..."}));
    lil_wayne->after = lil_wayne_after;
    lil_wayne_inside->add_child(lil_wayne);
    

    Object* vacant_lot = new Object("vacant_lot");
    vacant_lot->pretty_name = "a Vacant Lot";
    vacant_lot->add_component(new ComponentRoom({{EAST, "compton_street_north"}}));
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
    magdalene_lane->add_component(music_lucini);
    world->add_child(magdalene_lane);

    Object* garbage_alley = new Object("garbage_alley");
    garbage_alley->pretty_name = "a dim alley overflowing with garbage";
    garbage_alley->add_component(new ComponentRoom(
                {{SOUTH, "magdalene_lane"},
                {UP, "can tops"}})),
    garbage_alley->add_component(music_lucini);
    world->add_child(garbage_alley);

    Object* garbage_cans = new Object("garbage cans");
    garbage_cans->aliases = {"cans", "garbage", "rubbish", "bins", "can", "bin"};
    garbage_cans->add_component(new ComponentDescription("Several cans huddle by the wall in a pool of sodium-vapor light."));
    garbage_alley->add_child(garbage_cans);

    Object* can_tops = new Object("can tops");
    can_tops->pretty_name = "the top of the garbage cans";
    can_tops->add_component(new ComponentDescription("They wobble precariously beneath your feet."));
    can_tops->add_component(new ComponentRoom(
                {{DOWN, "garbage_alley"},
                {UP, "roof"}}));
    world->add_child(can_tops);

    Object* drain_pipe = new Object("drain pipe");
    drain_pipe->aliases = {"pipe", "drain"};
    drain_pipe->add_component(new ComponentDescription("A drain pipe runs down the wall on one side."));
    can_tops->add_child(drain_pipe);

    Object* roof = new Object("roof");
    roof->pretty_name = "the roof";
    roof->add_component(new ComponentRoom({{DOWN, "can tops"}}));
    roof->add_component(new ComponentDescription("You hear the regular dripping of water. The roof is dark and tarry."));
    world->add_child(roof);

    Object* rope = new Object("rope");
    rope->add_component(new ComponentTakeable());
    rope->add_component(new ComponentTie());
    rope->add_component(new ComponentDescription("A coil of rope lies to one side."));
    roof->add_child(rope);

    Object* kolob_street = new Object("kolob_street");
    kolob_street->pretty_name = "Kolob Street";
    kolob_street->add_component(new ComponentRoom({{EAST, "kolob_street_east"},
                {SOUTH, "jamal_front"},
                {WEST, "magdalene_lane"}}));
    world->add_child(kolob_street);

    Object* jamal_front = new Object("jamal_front");
    jamal_front->pretty_name = "the front of Jamal's house";
    jamal_front->add_component(new ComponentRoom({{NORTH, "kolob_street_east"},
                {SOUTH, "jamal_corridor"}}));
    jamal_front->before = jamal_front_before;
    world->add_child(jamal_front);

    kolob_street_east = new Object("kolob_street_east");
    kolob_street_east->pretty_name = "East Kolob Street";
    kolob_street_east->add_component(new ComponentRoom({{WEST, "kolob_street"}}));
    world->add_child(kolob_street_east);

    Object* shaft = new Object("shaft");
    shaft->pretty_name = "a ledge overlooking a deep shaft";
    shaft->add_component(new ComponentRoom({{WEST, "sewer"}}));
    shaft->add_component(new ComponentDescription("You see sunlight filtering down from a grate high above your head.\nRumbling and clanking sounds drift up from the darkness below."));
    world->add_child(shaft);

    Object* grate = new Object("grate");
    grate->add_component(new ComponentDescription("A heavy grate is inset in the road.", "You could tie something around its bars..."));
    grate->add_component(new ComponentTieTo());
    /*grate->after = [&](Command* cmd) {
        if(cmd->type == Command::TIE_TO)
        {
            static_cast<ComponentRoom*>(shaft->get_component(Component::ROOM))->
                directions[DOWN] = "subway_tunnel";
            static_cast<ComponentDescription*>(shaft->get_component(Component::DESCRIPTION))->
                current_appearance += "\nA rope dangles down from the grate into the shaft.";
            engine->terminal->disp("The rope dangles down into the dark shaft.");
        }
    };*/
    kolob_street_east->add_child(grate);

    Object* shaft_view = new Object("shaft");
    shaft_view->add_component(new ComponentDescription("Beneath the grate is a deep shaft going into the ground. Rumbling and clanking sounds drift up from the darkness below."));
    grate->add_child(shaft_view);

    Object* subway_tunnel = new Object("subway_tunnel");
    /*subway_tunnel->after = [&](Command* cmd) {
        if(cmd->type == Command::LOOK_AROUND && world->get_flag("subway_outcome") == 0)
        {
            engine->terminal->disp("You drop into the subway and land atop a moving train.");
            engine->terminal->pause();
            engine->terminal->disp("Press the up key to jump gaps between cars and down to duck beneath low ceilings.");
            engine->terminal->pause();
            //text->send_front(std::make_shared<CmdAddGameState>(new GameStateSubway(engine)));
            world->set_flag("subway_outcome", 1);
            world->set_current_room("subway_station");
        }
        return true;
    };*/
    world->add_child(subway_tunnel);

    Object* subway_station = new Object("subway_station");
    ComponentMusic* weeb_music = new ComponentMusic("res/shoujo.ogg");
    subway_station->pretty_name = "Sakura Street Subway Station";
    subway_station->add_component(new ComponentDescription("This deserted station reeks of stale urine and the ubiquitous cherry blossoms."));
    subway_station->add_component(new ComponentRoom({{NORTH, "sakura_park"}}));
    subway_station->add_component(new ComponentMusic("res/blackandyellow.ogg"));
    world->add_child(subway_station);

    Object* sakura_park = new Object("sakura_park");
    sakura_park->pretty_name = "Sakura Park";
    sakura_park->add_component(new ComponentDescription("An idyllic round park."));
    sakura_park->add_component(new ComponentMusic("res/blackandyellow.ogg"));
    sakura_park->add_component(new ComponentRoom({{SOUTH, "subway_station"},
                                                  {NORTH, "evangelion_ct"},
                                                  {EAST, "barber_shoppe"},
                                                  {WEST, "hobby_lobby_floor_1"}}));
    world->add_child(sakura_park);

    Object* cherry_trees = new Object("cherry trees");
    cherry_trees->add_component(new ComponentDescription("A stand of cherry trees blossom in the center of the park."));
    sakura_park->add_child(cherry_trees);

    Object* evangelion_ct = new Object("evangelion_ct");
    evangelion_ct->pretty_name = "Evangelion Court";
    evangelion_ct->add_component(new ComponentDescription("This boulevard connects Sakura Park to simple Ted's house. Simple Ted's house is to the north."));
    evangelion_ct->add_component(new ComponentRoom({{NORTH, "simple_ted_house_front"}, {SOUTH, "sakura_park"}}));
    world->add_child(evangelion_ct);

    Object* simple_ted_house_front = new Object("simple_ted_house_front");
    simple_ted_house_front->pretty_name = "The front of Simple Ted's house";
    simple_ted_house_front->add_component(new ComponentRoom({{SOUTH, "evangelion_ct"}}));
    world->add_child(simple_ted_house_front);

    Object* front_door = new Object("front door");
    front_door->aliases = {"door", "front"};
    front_door->add_component(new ComponentOpenClose(false));
    front_door->add_component(new ComponentPortal("simple_ted_house_first_floor"));
    front_door->add_component(new ComponentDescription("A crude door leading into the house is bolted shut."));
    /*front_door->before = [&](Command* cmd) {
        bool ret_val = false;
        if((cmd->type == Command::GO && static_cast<CmdGo*>(cmd)->new_room == "simple_ted_house_first_floor")
                || (cmd->type == Command::OPEN))
        {
            engine->terminal->disp("The door is bolted shut from the inside and won't budge.");
            ret_val = false;
        }
        else
            ret_val = true;
        return ret_val;
    };*/
    simple_ted_house_front->add_child(front_door);

    Object* second_floor_window = new Object("second floor window");
    second_floor_window->aliases = {"window"};
    second_floor_window->add_component(new ComponentDescription("A single window overlooks the yard from the second floor."));
    simple_ted_house_front->add_child(second_floor_window);

    Object* stones = new Object("gallstones");
    stones->aliases = {"stones", "gall", "rocks", "gallstone", "stone", "rock"};
    stones->add_component(new ComponentDescription("A pile of giant gallstones rests on the ground.", "The stones are heavy, but small enough to throw."));
    stones->add_component(new ComponentTakeable());
    simple_ted_house_front->add_child(stones);

    Object* pulley = new Object("pulley baskets");
    pulley->aliases = {"pulley", "baskets"};
    pulley->add_component(new ComponentDescription("There are two large baskets on a pulley system, one at the level of Ted's window and one on the ground."));
    pulley->add_component(new ComponentRoom({{DOWN, "simple_ted_house_front"}}));
    simple_ted_house_front->add_child(pulley);

    Object* hobby_lobby_floor_1 = new Object("hobby_lobby_floor_1");
    hobby_lobby_floor_1->pretty_name = "Hobby Lobby: 1st floor";
    hobby_lobby_floor_1->add_component(new ComponentDescription("It exceeds the wildest imaginination of the hobby rocketeer and the budding Japanophile."));
    hobby_lobby_floor_1->add_component(new ComponentMusic("res/weed.ogg"));
    hobby_lobby_floor_1->add_component(new ComponentRoom({{EAST, "sakura_park"},
                {WEST, "hobby_elevator"}}));
    world->add_child(hobby_lobby_floor_1);

    Object* hobby_lobby_floor_2 = new Object("hobby_lobby_floor_2");
    hobby_lobby_floor_2->pretty_name = "Hobby Lobby: 2nd floor";
    hobby_lobby_floor_2->add_component(new ComponentDescription("The second floor of the lobby"));
    hobby_lobby_floor_2->add_component(weeb_music);
    hobby_lobby_floor_2->add_component(new ComponentRoom({{WEST, "hobby_elevator"}}));
    world->add_child(hobby_lobby_floor_2);

    Object* store_shelf = new Object("shelf");
    store_shelf->aliases = {"shelves"};
    store_shelf->add_component(new ComponentDescription("Several large shelves run the length of the room."));
    hobby_lobby_floor_2->add_child(store_shelf);

    Object* onahole = new Object("onahole");
    onahole->add_component(new ComponentTakeable());
    onahole->add_component(new ComponentDescription("There is an onahole here.", "NEKOMIMI MAONYAN SPECIAL CATGIRL ONAHOLE"));
    /*onahole->after = [&](Command* cmd) {
        engine->terminal->disp(">lewd");
    };*/
    store_shelf->add_child(onahole);

    Object* touhou_figurine = new Object("touhou figurine");
    touhou_figurine->aliases = {"touhou", "figurine", "figure"};
    touhou_figurine->add_component(new ComponentTakeable());
    touhou_figurine->add_component(new ComponentDescription("There is a Touhou figurine here.", "Patchouli Knowledge is finally getting her own Nendoroid!"));
    store_shelf->add_child(touhou_figurine);

    Object* honami_aihara_figurine = new Object("honami aihara figurine");
    honami_aihara_figurine->aliases = {"honami", "aihara", "figurine", "figure"};
    honami_aihara_figurine->add_component(new ComponentTakeable());
    honami_aihara_figurine->add_component(new ComponentDescription("There is a Honami Aihara figurine here.", "She is wearing a super sexy idol uniform while squatting with legs spread and making a heart shaped symbol with her hands in front of her crotch just like the cover illustration of Ima Ria."));
    store_shelf->add_child(honami_aihara_figurine);

    body_pillow = new Object("Oreimo Kirino body pillow");
    body_pillow->aliases = {"oreimo", "kirino", "body", "pillow"};
    body_pillow->add_component(new ComponentDescription("There is a Kirino body pillow here.", "New Oreimo Kirino Kousaka Soft Dakimakura Hugging Body Pillow"));
    body_pillow->add_component(new ComponentTakeable());

    store_shelf->add_child(body_pillow);

    Object* hobby_lobby_floor_3 = new Object("hobby_lobby_floor_3");
    hobby_lobby_floor_3->pretty_name = "Hobby Lobby: 3rd floor";
    hobby_lobby_floor_3->add_component(new ComponentDescription("The third floor of the lobby"));
    hobby_lobby_floor_3->add_component(weeb_music);
    hobby_lobby_floor_3->add_component(new ComponentRoom({{WEST, "hobby_elevator"}}));
    world->add_child(hobby_lobby_floor_3);

    Object* hobby_window = new Object("window");
    hobby_window->add_component(new ComponentDescription("There is one window on the west wall, through which the evening glow lights this store's salacious wares."));
    hobby_window->add_component(new ComponentOpenClose(true));
    hobby_window->add_component(new ComponentPortal("fire_escape"));
    hobby_lobby_floor_3->add_child(hobby_window);

    Object* fire_escape = new Object("fire_escape");
    fire_escape->pretty_name = "Hobby Lobby: fire escape";
    fire_escape->add_component(new ComponentRoom({{EAST, "hobby_lobby_floor_3"},
                                                  {DOWN, "kolob_street_east"}}));
    fire_escape->add_component(new ComponentDescription("You're running up in gates, and doing hits for high stakes,\nMaking [your] way on fire escapes\nYou can see the street about twenty feet below."));
    fire_escape->before = fire_escape_before;
    fire_escape->after = fire_escape_after;

    world->add_child(fire_escape);

    Object* hobby_elevator = new Object("hobby_elevator");
    hobby_elevator->pretty_name = "Hobby Lobby Elevator";
    //hobby_elevator->add_component(new ComponentRoom({{}}));
    hobby_elevator->add_component(new ComponentMusic("res/weed.ogg"));
    world->add_child(hobby_elevator);

    Object* elevator_buttons = new Object("button panel");
    elevator_buttons->aliases = {"buttons", "panel", "button"};

    elevator_buttons->add_component(new ComponentDescription("A panel of buttons is on the wall of the elevator.", "There are three buttons, labeled '3', '2', and '1', respectively."));
    elevator_buttons->add_component(new ComponentHittable());
    /*elevator_buttons->after = [&](Command* cmd) {
        if(cmd->type == Command::HIT)
        {
            engine->push_state(new GameStateMenu(engine, text, "There are three buttons: 3, 2, and 1. Which button do you hit?",
                        {{"1", {std::make_shared<CmdGo>("hobby_lobby_floor_1")}},
                         {"2", {std::make_shared<CmdGo>("hobby_lobby_floor_2")}},
                         {"3", {std::make_shared<CmdGo>("hobby_lobby_floor_3")}}}));
        }
    };*/
    hobby_elevator->add_child(elevator_buttons);

    Object* barber_shoppe = new Object("barber_shoppe");
    barber_shoppe->pretty_name = "Frank's Barber Shoppe";
    barber_shoppe->add_component(new ComponentDescription("The shoppe is a dingy, sparsely-furnished room."));
    ComponentMusic* italian_music = new ComponentMusic("res/italian.ogg");
    barber_shoppe->add_component(italian_music);
    barber_shoppe->add_component(new ComponentRoom({{WEST, "sakura_park"}}));
    world->add_child(barber_shoppe);

    Object* frank = new Object("frank");
    frank->aliases = {"barber", "man", "him", "he"};
    frank->add_component(new ComponentDescription("Frank reclines in a chair, playing the accordian"));
    ComponentTalkable* frank_talk = new ComponentTalkable({"I believe in my bloodclart barber",
                "There's levels to this ting and when manna trim they level affi set",
                "Yo, It's barely been a week and I wanna go and get a trim again",
                "-You want the haircut?",
                "not really mane.",
                });
    frank->add_component(frank_talk);
    barber_shoppe->add_child(frank);

    Object* clippers = new Object("clippers");
    clippers->aliases = {"hedge clippers", "hedge", "scissors"};
    clippers->add_component(new ComponentDescription("Frank is holding a pair of shiny hedge clippers."));
    clippers->add_component(new ComponentTakeable());
    /*clippers->before = [&](Command* cmd) {
        if(cmd->type == Command::TAKE)
        {
            engine->terminal->disp("Frank is startled by your gall.");
            frank_talk->talkable_data = {"-I'm startled by your gall, boy.",
                "i din do nuffin",
                "-If you want the clippers, at least pay the price.",
                "How many shekels?",
                "-Shekels cannot buy these adamantine blades.",
                "-They can only be purchased with blank media."};
            //std::shared_ptr<CmdTalkTo> talk_to = std::make_shared<CmdTalkTo>();
            //talk_to->add_object(frank);
            //text->send_front(talk_to);
            return false;
        }
        return true;
    };
    frank->add_child(clippers);
    */


    return world;
}
