#include "TextReferencable.h"

TextReferencable::TextReferencable(std::string name_in, std::vector<std::string> aliases_in)
	: name(name_in), aliases(aliases_in)
{
}

TextReferencable::TextReferencable()
: name(), aliases()
{
}


TextReferencable::~TextReferencable()
{
}

std::string TextReferencable::get_name()
{
	return name;
}

void TextReferencable::set_name(std::string name_in)
{
	name = name_in;
}

std::vector<std::string> TextReferencable::get_aliases()
{
	return aliases;
}

void TextReferencable::set_aliases(std::vector<std::string> aliases_in)
{
	aliases = aliases_in;
}