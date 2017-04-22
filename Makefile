UNAME := $(shell uname -s)

EXE = gemu
SOURCES = $(wildcard *.cpp)
OBJS = $(SOURCES:.cpp=.o)

ifeq ($(UNAME), Darwin)
	CXXFLAGS = -F/Library/Frameworks -framework SFML -framework sfml-window -framework sfml-graphics -framework sfml-system -framework Cocoa
else
	CXXFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
endif

CXXFLAGS += -std=c++11


$(EXE): $(OBJS)
	$(CXX) $^ -o $(EXE) $(CXXFLAGS)

clean:
	rm -r $(OBJS) $(EXE)
