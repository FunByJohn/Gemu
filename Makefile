EXE = gemu
SOURCES = $(wildcard *.cpp)
OBJS = $(SOURCES:.cpp=.o)
CXXFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

$(EXE): $(OBJS)
	$(CXX) $^ -o $(EXE) $(CXXFLAGS)

clean:
	rm -r $(OBJS) $(EXE)
