#pragma once
#include "Directions.h"
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
        std::string shallow_description;
        std::string deep_description;
        bool show_children;
        ComponentDescription(std::string shallow_in, std::string deep_in = "")
            : Component(DESCRIPTION),
            shallow_description(shallow_in),
            deep_description(deep_in),
            show_children(true)
        { }
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

typedef std::shared_ptr<Component> ComponentPtr;
