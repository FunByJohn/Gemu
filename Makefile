UNAME := $(shell uname -s)

EXE = gemu
SOURCES = $(wildcard *.cpp)
OBJS = $(SOURCES:.cpp=.o)
DEP = $(OBJS:%.o=%.d)

ifeq ($(UNAME), Darwin)
	CXXFLAGS = -F/Library/Frameworks -framework SFML -framework sfml-window -framework sfml-graphics -framework sfml-system -framework Cocoa
else
	CXXFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
endif

CXXFLAGS += -std=c++14


$(EXE): $(OBJS)
	@echo $(DEP)
	$(CXX) $^ -o $(EXE) $(CXXFLAGS)

-include $(DEP)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

clean:
	rm -r $(OBJS) $(EXE)


