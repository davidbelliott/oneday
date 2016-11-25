#include "World.h"
#include "Event.h"
#include "Object.h"
#include "GameState.h"
#include "Command.h"
#include "Terminal.h"

Object::Object(std::string name_in)
	: name(name_in),
      pretty_name(name_in),
      parent(nullptr),
      components(),
      pre_command(nullptr),
      post_command(nullptr)
{
}

Object::Object()
	: Object("")
{

}

Object::~Object()
{
}

void Object::add_child(Object* child)
{
	child->parent = this;
	children.push_back(child);
	children_hash[child->name].push_back(child);
	for (size_t i = 0; i < child->aliases.size(); i++)
		children_hash[child->aliases[i]].push_back(child);
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
	if (has_direct_child(name))
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
    name = name_in;
}

void Object::add_component(Component* component)
{
    components[component->type] = component;
}

void Object::rm_component(Component::Type type)
{
    components.erase(type);
}

bool Object::has_component(Component::Type type)
{
    return (components.count(type) >= 1);
}

Component* Object::get_component(Component::Type type)
{
    if(has_component(type))
        return components[type];
    else
        return nullptr;
}

/*void Object::describe(GameState* g, bool deep, bool describe_this)
{
    if(properties & Object::ROOM)
    {
        g->terminal->set_color(config::colors[config::color_room_title]);
        g->terminal->disp("You in " + pretty_name + ".");
        g->terminal->set_color();
    }
    if(describe_this && (properties & Object::VISIBLE))
    {
        if (deep && !deep_description.empty())
            g->terminal->disp(deep_description);
        else
            g->terminal->disp(shallow_description);
    }
    if(properties & Object::ROOM)
    {
        for(int i = 0; i < DIRECTION_MAX; i++)
        {
            if(directions[i] != "")
            {
                DirectionId dir_id = (DirectionId)i;
                Object* dir_room = parent->get_direct_child(directions[i], 0);
                if(dir_room && dir_room->pretty_name != "")
                {
                    std::string dir_reference = dir[dir_id].dir_reference;
                    g->terminal->disp(dir_reference + " is " + dir_room->pretty_name + ".");
                }
            }
        }
    }
    // If this isn't a container and show_children is true, show the children;
    // If this is a container and it's open, show the children.
    if ((!(properties & Object::CONTAINER) && show_children) || ((properties & Object::CONTAINER) && open))
    {
        for (int j = 0; j < children.size(); j++)
        {
            // If it's a deep description, show all children.
            // Otherwise, don't show the undiscovered children.
            if(deep || (children[j]->properties & Object::DISCOVERED))
            {
                g->terminal->disp(children[j]->shallow_description);
                children[j]->properties |= Object::DISCOVERED;
            }
        }
    }
    properties |= Object::DISCOVERED;
}*/
