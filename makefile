CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
CC_FLAGS := -g
bin/oneday: $(OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp
	g++  $(CC_FLAGS) -c -o $@ $<
