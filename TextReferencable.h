#pragma once

#include <string>
#include <vector>

class TextReferencable
{
private:
	std::string name;
	std::vector<std::string> aliases;
public:
	TextReferencable(std::string name_in, std::vector<std::string> aliases_in);
	TextReferencable();
	virtual ~TextReferencable();

	std::string get_name();
	void set_name(std::string name_in);
	
	std::vector<std::string> get_aliases();
	void set_aliases(std::vector<std::string> aliases_in);
};

