#include "World.h"
#include "Event.h"
#include "Object.h"
#include "GameState.h"
#include "Command.h"

Object::Object(std::string name_in, std::string description_in)
	: name(name_in, {""}, Word::OBJECT), pretty_name(""), parent(NULL), shallow_description(description_in), properties(VISIBLE), show_children(true)
{
}

Object::Object()
	: Object("", "")
{

}

Object::~Object()
{
}

void Object::describe(GameState* g, bool deep, bool describe_this)
{
	if (describe_this && (properties & VISIBLE))
	{
		if (deep && !deep_description.empty())
			g->send(std::make_shared<CmdDisp>(deep_description));
		else
			g->send(std::make_shared<CmdDisp>(shallow_description));
	}
    // If this isn't a container and show_children is true, show the children;
    // If this is a container and it's open, show the children.
	if ((!(properties & CONTAINER) && show_children) || ((properties & CONTAINER) && open))
	{
		for (size_t i = 0; i < children.size(); i++)
		{
            // If it's a deep description, show all children.
            // Otherwise, don't show the undiscovered children.
			if(deep || (children[i]->properties & DISCOVERED))
				children[i]->describe(g, false, true);
		}
	}
    properties |= DISCOVERED;
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
    for(auto it = children_hash.begin(); it != children_hash.end(); )
    {
        it->second.erase(std::remove(it->second.begin(), it->second.end(), child), it->second.end());
        if(it->second.empty())
            children_hash.erase(it++);
        else
            ++it;
    }
}

bool Object::has_direct_child(std::string name)
{
	return children_hash.count(name) > 0;
}

Object* Object::get_direct_child(std::string name, int filter)
{
	Object* child = NULL;
	if (has_direct_child(name) && (children_hash[name][0]->properties & filter) == filter)
		return children_hash[name][0];
	else
		return NULL;
}

Object* Object::get_indirect_child(std::string name, int filter)
{
	Object* child = get_direct_child(name, filter);
	if (child)
		return child;
	else
	{
		for (size_t i = 0; i < children.size() && !child; i++)
		{
			child = children[i]->get_indirect_child(name, filter);
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
