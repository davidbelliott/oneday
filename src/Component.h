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
            ROOM
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
        ComponentDescription(std::string shallow_in)
            : Component(DESCRIPTION),
            shallow_description(shallow_in),
            deep_description(shallow_in),
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
        ComponentRoom()
            : Component(ROOM),
            directions()
    { }
};

typedef std::shared_ptr<Component> ComponentPtr;
