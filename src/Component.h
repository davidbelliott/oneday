#pragma once
#include "Directions.h"
#include <memory>
#include <set>

class Object;

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
            MOVEABLE,
            MUSIC,
            OPEN_CLOSE,
            INVENTORY,
            TIE,
            TIE_TO
        } type;
        Component(Type type_in)
            : type(type_in)
        { }
        virtual ~Component()
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
        std::map<DirectionId, std::string> directions;
        std::set<DirectionId> hidden;
        ComponentRoom(std::map<DirectionId, std::string> directions_in, std::set<DirectionId> hidden_in = {})
            : Component(ROOM),
            directions(directions_in),
            hidden(hidden_in)
    {
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

class ComponentMoveable : public Component
{
    public:
        Object* new_parent;
        ComponentMoveable(Object* new_parent_in=nullptr)
            : Component(MOVEABLE),
            new_parent(new_parent_in)
        {
        }
};

class ComponentMusic : public Component
{
    public:

        std::string music;
        bool persistent;

        ComponentMusic(std::string filename_in)
            : Component(MUSIC),
            music(filename_in)
        {
        }
};

class ComponentInventory : public Component
{
    public:
        
        ComponentInventory()
            : Component(INVENTORY)
        {
        }
};

class ComponentOpenClose : public Component
{
    public:
        bool open;

        ComponentOpenClose(bool open_in = false)
            : Component(OPEN_CLOSE),
            open(open_in)
        {
        }
};

class ComponentTie : public Component
{
    public:
        std::vector<Object*> tie_to;
        
        ComponentTie(std::vector<Object*> tie_to_in={})
            : Component(TIE),
            tie_to(tie_to_in)
        { }
};

class ComponentTieTo : public Component
{
    public:
        std::vector<Object*> tie;

        ComponentTieTo(std::vector<Object*> tie_in={})
            : Component(TIE_TO),
            tie(tie_in)
        { }
};

typedef std::shared_ptr<Component> ComponentPtr;
