CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := -lncurses# `pkg-config --libs caca` #-I ../libcaca ../libcaca/src
CC_FLAGS := #-lncurses `pkg-config --cflags caca` #-I ../libcaca ../libcaca/src

all: bin/oneday

bin/oneday: $(OBJ_FILES)
	g++ $(OBJ_FILES) $(LD_FLAGS) -o bin/oneday

obj/%.o: src/%.cpp
	g++ -c $< $(CC_FLAGS) -o $@

clean:
	rm obj/* bin/oneday
