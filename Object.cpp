#include "Object.h"
#include "Terminal.h"

Object::Object(std::string name_in, std::string description_in)
	: name(name_in, {""}, Word::OBJECT), parent(NULL), shallow_description(description_in), discovered(false), properties(VISIBLE), show_children(true)
{
	run_action = [](World*, Terminal*, Action*, Object*) { return true; };
}

Object::Object()
	: Object("", "")
{

}

Object::~Object()
{
}

void Object::describe(Terminal* t, bool deep, bool describe_this)
{
	if (describe_this && (properties & VISIBLE))
	{
		if (deep && !deep_description.empty())
			t->disp(deep_description);
		else
			t->disp(shallow_description);
	}
	if ((!(properties & CONTAINER) && show_children) || ((properties & CONTAINER) && open))
	{
		for (size_t i = 0; i < children.size(); i++)
		{
			if(deep || children[i]->discovered)
				children[i]->describe(t, false, true);
		}
	}
	discovered = true;
}

void Object::add_child(Object* child)
{
	child->parent = this;
	children.push_back(child);
	children_hash[child->name.word].push_back(child);
	for (size_t i = 0; i < child->name.aliases.size(); i++)
		children_hash[child->name.aliases[i]].push_back(child);
}

void Object::remove_child(Object* child)
{
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

bool Object::has_direct_child(std::string name)
{
	return children_hash.count(name) > 0;
}

Object* Object::get_direct_child(std::string name, bool filter_discovered)
{
	Object* child = NULL;
	if (has_direct_child(name) && (!filter_discovered || children_hash[name][0]->discovered))
		return children_hash[name][0];
	else
		return NULL;
}

Object* Object::get_indirect_child(std::string name, bool filter_discovered)
{
	Object* child = get_direct_child(name, filter_discovered);
	if (child)
		return child;
	else
	{
		for (size_t i = 0; i < children.size() && !child; i++)
		{
			child = children[i]->get_indirect_child(name, filter_discovered);
		}
		return child;
	}
}

int Object::get_flag(std::string name)
{
	return flags[name];
}

void Object::set_flag(std::string name, int value)
{
	flags[name] = value;
}

void Object::set_name(std::string name_in)
{
    name.word = name_in;
}
