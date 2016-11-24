#include "File.h"
#include <fstream>
#include <algorithm>

std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    std::string contents = "";
    if (in)
    {
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
    }
    std::replace(contents.begin(), contents.end(), '@', '\0');
    return(contents);
}
