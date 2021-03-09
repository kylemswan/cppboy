# EXECUTABLE NAME
EXE := cppboy

# FILE DIRECTORIES
SRCDIR := source
INCDIR := include
BLDDIR := build

# TOOLCHAIN CONFIGURATION AND OPTIONS
CXX := g++
CXXFLAGS := -g -Wall -MMD -I$(INCDIR)
LDLIBS := -lsfml-system -lsfml-window -lsfml-graphics
VPATH := source : source/CPU

# DYNAMICALLY GENERATED INPUT FILES
SRCS := $(notdir $(shell find source/ -name *.cpp))
OBJS := $(patsubst %.cpp, $(BLDDIR)/%.o, $(SRCS))
DEPS := $(wildcard build/*.d)

# COMPILATION AND LINKING TARGETS
$(EXE): $(OBJS)
	$(CXX) $^ -o $@ $(LDLIBS)

$(BLDDIR)/%.o: %.cpp | $(BLDDIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS) 

$(BLDDIR):
	mkdir $(BLDDIR)

include $(DEPS)

# UTILITY TARGETS
run:
	./$(EXE)

clean:
	rm -rf $(BLDDIR)

remove:
	rm -f $(EXE)
