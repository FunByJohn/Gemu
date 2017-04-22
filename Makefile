UNAME := $(shell uname -s)

EXE = gemu
SOURCES = $(wildcard *.cpp)
OBJS = $(SOURCES:.cpp=.o)
DEP = $(OBJS:%.o=%.d)

ifeq ($(UNAME), Darwin)
	CXXFLAGS = -F/Library/Frameworks -framework SFML -framework sfml-window -framework sfml-graphics -framework sfml-system -framework Cocoa -Wno-c++11-narrowing -DVSYNC
else
	CXXFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
endif

CXXFLAGS += -std=c++14 -Wno-narrowing

ifdef DEBUG
	CXXFLAGS += -O0 -ggdb
endif


$(EXE): $(OBJS)
	$(CXX) $^ -o $(EXE) $(CXXFLAGS)

-include $(DEP)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

clean:
	rm -r $(DEP) $(OBJS) $(EXE)
