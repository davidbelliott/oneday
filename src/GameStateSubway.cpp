#include "GameStateSubway.h"
#include "GameStateThugFight.h"
#include "File.h"
#include "Audio.h"
#include "Engine.h"
#include "Terminal.h"

GameStateSubway::GameStateSubway(Engine* engine_in)
    : GameState(engine_in)
{
}

GameStateSubway::~GameStateSubway()
{
}

void GameStateSubway::load_beats()
{
    terrain.clear();
    for(int i = 0; i < total_beats; i++)
    {
        terrain.push_back((Terrain)(rand() % 3));
    }
}

void GameStateSubway::init()
{
    cur_beat = 0;
    total_beats = 150;
    elapsed_time = sf::seconds(0.0);
    load_beats();
    beat = sf::seconds(60.0 / 165.5);
    player = { 20.0, 0.0, get_file_contents("subway_player") };
    music = new Music("res/subway.ogg");
    send(std::make_shared<CmdPlayMusic>(music));
}

void GameStateSubway::cleanup()
{
    std::make_shared<CmdStopMusic>(music)->run(this);
}

void GameStateSubway::notify(event_ptr event)
{


}

int GameStateSubway::get_seconds_remaining()
{
    return std::max(0, round((total_beats - cur_beat) * beat.asSeconds()));
}

int GameStateSubway::get_x(int x_beat)
{
    int offset = x_beat * 10;
    int cur_pos = int((cur_beat - 2) * 10);
    return offset - cur_pos;
}


void GameStateSubway::update(sf::Time dt)
{
    if(!paused)
    {

        if(cur_beat > total_beats)
        {
            win();
        }
        
        elapsed_time += dt;
        cur_beat = (elapsed_time.asSeconds() / beat.asSeconds());
    }
}

void GameStateSubway::draw()
{
    engine->terminal->clr();

    // Draw walls
    for(int x, y, i = std::max(0, (int)cur_beat - 2); i < std::min(total_beats, (int)cur_beat + 7); i++)
    {
        x = get_x(i);
        std::string top;
        std::string bot;
        if(terrain[i] == NORMAL)
        {
            top = "";
            bot = "##########";
        }
        else if(terrain[i] == LOW_CEILING)
        {
            top = "##########";
            bot = "##########";
        }
        else if(terrain[i] = NO_FLOOR)
        {
            top = "";
            bot = "";
        }
        for(int j = 0; j < config::screen_h_chars; j++)
        {
            if(j < 10)
                engine->terminal->output(x, config::screen_h_chars - j - 1, bot);
            else if(j > 12)
                engine->terminal->output(x, config::screen_h_chars - j - 1, top);
        }
    }

}

void GameStateSubway::win()
{

}

void GameStateSubway::lose()
{

}

