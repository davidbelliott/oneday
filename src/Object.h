#pragma once
#include "Word.h"
#include "Component.h"

class Object
{
public:
	Object* parent;
	std::vector<Object*> children;
	std::map<std::string, std::vector<Object*>> children_hash;
	std::map<std::string, int> flags;
    std::map<Component::Type, ComponentPtr> components;

    Word name;
    std::string pretty_name;

	Object(std::string name_in);
	Object();
	virtual ~Object();

    void add_component(ComponentPtr component);
    void rm_component(Component::Type type);

    bool has_component(Component::Type type);
    ComponentPtr get_component(Component::Type type);


	void add_child(Object* child);
	void remove_child(Object* child);
	bool has_direct_child(std::string name);
	Object* get_direct_child(std::string name, int filter);
	Object* get_indirect_child(std::string name, int filter);
	int get_flag(std::string name);
	void set_flag(std::string name, int value);
    void set_name(std::string name_in);
};

