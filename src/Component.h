#pragma once
#include "Directions.h"
#include <SFML/Audio.hpp>
#include <memory>

class Component
{
    public:
        enum Type
        {
            DESCRIPTION,
            TEXT,
            FLAMMABLE,
            EDIBLE,
            CONTAINER,
            HITTABLE,
            WEARABLE,
            TALKABLE,
            ROOM,
            PORTAL,
            TAKEABLE,
            CLIMBABLE,
            MUSIC
        } type;
        Component(Type type_in)
            : type(type_in)
        { }
        virtual ~Component()
        { }

        virtual void update(sf::Time dt)
        { }
};

class ComponentDescription : public Component
{
    public:
        std::string initial_appearance;    // Description shown when parent described
        std::string later_appearance;
        std::string current_appearance;
        std::string description;       // Description shown when this described
        bool show_children;
        ComponentDescription(std::string initial_in, std::string description_in = "", std::string later_in = "")
            : Component(DESCRIPTION),
            initial_appearance(initial_in),
            later_appearance(later_in),
            description(description_in),
            show_children(true)
        {
            current_appearance = initial_appearance;
        }
};

class ComponentText : public Component
{
    public:
        std::string text;
        ComponentText(std::string text_in)
            : Component(TEXT),
            text(text_in)
        { }
};

class ComponentFlammable : public Component
{
    public:
        bool burning;
        ComponentFlammable()
            : Component(FLAMMABLE),
            burning(false)
    { }
};

class ComponentEdible : public Component
{
    public:
        ComponentEdible()
            : Component(EDIBLE)
        { }
};

class ComponentContainer : public Component
{
    public:
        bool open;
        ComponentContainer()
            : Component(CONTAINER),
            open(false)
        { }
};

class ComponentHittable : public Component
{
    public:
        bool flipped;
        ComponentHittable()
            : Component(HITTABLE),
            flipped(false)
    { }
};

class ComponentWearable : public Component
{
    public:
        ComponentWearable()
            : Component(WEARABLE)
        { }
};

class ComponentTalkable : public Component
{
    public:
        std::vector<std::string> talkable_data;
        ComponentTalkable(std::vector<std::string> talkable_data_in)
            : Component(TALKABLE),
            talkable_data(talkable_data_in)
    { }
};

class ComponentRoom : public Component
{
    public:
        std::string directions[DIRECTION_MAX];
        ComponentRoom(std::map<DirectionId, std::string> directions_in)
            : Component(ROOM),
            directions()
    {
        for(int i = 0; i < DIRECTION_MAX; i++)
            directions[i] = "";
        for(auto it = directions_in.begin(); it != directions_in.end(); ++it)
            directions[int(it->first)] = it->second;
    }
};

class ComponentPortal : public Component
{
    public:
        std::string destination;
        ComponentPortal(std::string destination_in)
            : Component(PORTAL),
              destination(destination_in)
    { }
};

class ComponentTakeable : public Component
{
    public:
        int mass;
        ComponentTakeable()
            : Component(TAKEABLE),
            mass(1)
    { }
};

class ComponentClimbable : public ComponentRoom
{
    public:
        ComponentClimbable(std::map<DirectionId, std::string> directions_in)
            : ComponentRoom(directions_in)
        {
            type = CLIMBABLE;
        }
};

class ComponentMusic : public Component
{
    public:

        enum Fade
        {
            PLAY,
            PAUSE,
            STOP,
            NONE
        } fade;
        double cur_volume;
        double target_volume;
        double volume_step;
        bool persistent;

        sf::Music music;

        ComponentMusic(std::string filename_in, bool persistent_in = true)
            : Component(MUSIC),
            music(),
            fade(NONE),
            cur_volume(0),
            target_volume(0),
            volume_step(0),
            persistent(persistent_in)
        {
            music.openFromFile(filename_in);
            music.setLoop(true);
        }

        void update(sf::Time dt);

        void set_fade(Fade fade_in);
};

typedef std::shared_ptr<Component> ComponentPtr;
