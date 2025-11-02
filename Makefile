CXX := g++             # compiler used for all translation units
CXXFLAGS := -I.        # include current directory so headers in subfolders resolve
LDFLAGS := -lncurses   # link against ncurses to enable terminal UI features
SOURCES := main.cpp Setup/Setup.cpp Commands/Commands.cpp  # all source files required by the editor

mains: $(SOURCES)      # build target: compile sources into executable then launch it
	$(CXX) $(CXXFLAGS) $(SOURCES) -o main $(LDFLAGS)
	./main
