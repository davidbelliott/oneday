#pragma once
#include "Word.h"
#include "Component.h"
class Command;

enum Constraint
{
    IN,
    ON,
    UNDER,
    PRESENT
};

std::string get_descriptor(Constraint c, std::string parent_name);

class Object
{
public:

    // Object tree data
	Object* parent;
    Constraint constraint;
	std::vector<Object*> children;
	std::map<std::string, std::vector<Object*>> children_hash;

    // State data
	std::map<std::string, int> flags;
    std::map<Component::Type, Component*> components;
    bool active;
    bool discovered;

    // Name data
    std::string name;
    std::string pretty_name;
    std::vector<std::string> aliases;

    // Command callbacks
    std::function<bool(Command*)> before;

    // Called after parse.
    std::function<void(Command*)> after;

	Object(std::string name_in);
	Object();
	virtual ~Object();

    virtual void add_component(Component* component);
    virtual void rm_component(Component::Type type);

    virtual bool has_component(Component::Type type);
    virtual Component* get_component(Component::Type type);


	virtual void add_child(Object* child);
	virtual void remove_child(Object* child);
	virtual bool has_direct_child(std::string name);
	virtual Object* get_direct_child(std::string name, int filter);
	virtual Object* get_indirect_child(std::string name, int filter);
	virtual int get_flag(std::string name);
	virtual void set_flag(std::string name, int value);
    virtual void set_name(std::string name_in);
};

