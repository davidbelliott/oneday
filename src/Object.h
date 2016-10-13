#pragma once

class Action;
class World;
class Terminal;
class Object;
#include "Word.h"
#include "common.h"
#include <functional>
#include <vector>
#include <string>
#include <map>

typedef std::function<void(World*, Terminal*)> run_func;
typedef std::function<bool(World*, Terminal*, Action*, Object*)> ActionFunc;

class Object
{
public:
	Object* parent;
	std::vector<Object*> children;
	std::map<std::string, std::vector<Object*>> children_hash;
	std::map<std::string, int> flags;
public:

	enum Property
	{
		TAKEABLE = 0x1,
		VISIBLE = 0x2,
		READABLE = 0x4,
		FLAMMABLE = 0x8,
		EDIBLE = 0x10,
		GOABLE = 0x20,
		CONTAINER = 0x40,
		HITTABLE = 0x80,
		WEARABLE = 0x100,
        TALKABLE = 0x200,
        DISCOVERED = 0x400
	};

    Word name;
    std::string pretty_name;

	//If VISIBLE
	std::string shallow_description;
	std::string deep_description;
	bool show_children;
	virtual void describe(Terminal* t, bool deep, bool describe_this);

    //If READABLE
    std::string readable_data;  //The text that can be read

	//If GOABLE
	std::string goable_data;	//The room that going through this object leads to

	//If CONTAINER
	bool open;
	
	//If HITTABLE
	bool flipped;

    //If TALKABLE
    std::vector<std::string> talkable_data; //Vector of strings that will be displayed one after another when you talk to the object

	uint16_t properties;
	ActionFunc pre_action;
    ActionFunc post_action;


	Object(std::string name_in, std::string description_in);
	Object();
	virtual ~Object();

	void add_child(Object* child);
	void remove_child(Object* child);
	bool has_direct_child(std::string name);
	Object* get_direct_child(std::string name, int filter);
	Object* get_indirect_child(std::string name, int filter);

	int get_flag(std::string name);
	void set_flag(std::string name, int value);
    void set_name(std::string name_in);
};

