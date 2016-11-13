#include "World.h"
#include "Event.h"
#include "Object.h"
#include "GameState.h"
#include "Command.h"

Object::Object(std::string name_in, std::string description_in)
	: name(name_in, {""}, Word::OBJECT),
      pretty_name(""),
      parent(nullptr),
      shallow_description(description_in),
      properties(VISIBLE),
      show_children(true),
      pre_command(nullptr),
      post_command(nullptr)
{
}

Object::Object()
	: Object("", "")
{

}

Object::~Object()
{
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
