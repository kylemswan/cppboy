# CONFIGURATION
EXE := cppboy

SRCDIR := source
INCDIR := include
BLDDIR := build

CXX := g++
CXXFLAGS := -g -Wall -MMD -I$(INCDIR)

SRCS := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/CPU/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(BLDDIR)/%.o, $(SRCS))
DEPS := $(wildcard $(BLDDIR)/*.d)

# COMPILATION AND LINKING TARGETS
$(EXE): $(OBJS)
	$(CXX) $^ -o $@ $(LDLIBS)

$(BLDDIR)/%.o: $(SRCDIR)/%.cpp | $(BLDDIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS) 

$(BLDDIR)/CPU/%.o: $(SRCDIR)/CPU/%.cpp | $(BLDDIR)/CPU
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(BLDDIR):
	mkdir $(BLDDIR)

$(BLDDIR)/CPU:
	mkdir $(BLDDIR)/CPU

include $(DEPS)

# UTILITY TARGETS
run:
	./$(EXE)

clean:
	rm -rf $(BLDDIR)

remove:
	rm -f $(EXE)
