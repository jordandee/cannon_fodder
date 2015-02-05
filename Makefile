#Getting fancier with dem makes

CC=g++
#-g is -g2, for getting info for debugger, -g3 gets more info
CFLAGS=-c -std=c++11 -g
LIBS=-lSDL2 -lSDL2_image -lSDL2_ttf
SOURCES=main.cpp gameengine.cpp title.cpp level.cpp terrain.cpp math.cpp resources.cpp globals.cpp timer.cpp cannon.cpp ball.cpp score.cpp obstacle.cpp options.cpp
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=cannon_fodder.out

#make target executable from sources
all: $(SOURCES) $(TARGET)

#make target from objects
#$@ is target
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

#make object from source
#$@ is name of object file
#$< is name of prerequisite .cpp file
.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

help:
	@echo "make: execute first rule in the makefile (make all)"
	@echo "make all: makes all"
	@echo "make clean: get rid of compiled files"
	@echo "make help: see this"

clean:
	@rm -v $(TARGET) *.o
