# compiler configuration
CXX := g++
CXXFLAGS := -g -Wall -MMD -Iinclude/

# link SFML libraries
LDLIBS := -lsfml-system -lsfml-window -lsfml-graphics

# names of all source directories
DIRS := source source/cpu

# find source files and generate the corresponding object and dependency names
SRCS := $(foreach DIR, $(DIRS), $(notdir $(wildcard $(DIR)/*.cpp)))
OBJS := $(patsubst %.cpp, build/%.o, $(SRCS))
DEPS := $(wildcard build/*.d)

# set up VPATH so that files can be found in their respective (sub) directories
VPATH := $(DIRS)

# compilation and linking targets
cppboy: $(OBJS)
	$(CXX) $^ -o $@ $(LDLIBS)

build/%.o: %.cpp | build/
	$(CXX) -c $< -o $@ $(CXXFLAGS) 

build/:
	mkdir build/

include $(DEPS)

# utility targets
run:
	./cppboy

clean:
	rm -rf build/

remove:
	rm -f cppboy
