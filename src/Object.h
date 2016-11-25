#pragma once
#include "Word.h"
#include "Component.h"
class Command;

class Object
{
public:

    // Object tree data
	Object* parent;
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
    std::function<bool(Command*)> pre_command;
    std::function<void(Command*)> post_command;

	Object(std::string name_in);
	Object();
	virtual ~Object();

    void add_component(Component* component);
    void rm_component(Component::Type type);

    bool has_component(Component::Type type);
    Component* get_component(Component::Type type);


	void add_child(Object* child);
	void remove_child(Object* child);
	bool has_direct_child(std::string name);
	Object* get_direct_child(std::string name, int filter);
	Object* get_indirect_child(std::string name, int filter);
	int get_flag(std::string name);
	void set_flag(std::string name, int value);
    void set_name(std::string name_in);
};

