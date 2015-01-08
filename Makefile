#Getting fancier with dem makes

CC=g++
CFLAGS=-c -std=c++11
LIBS=-lSDL2 -lSDL2_image
SOURCES=main.cpp gameengine.cpp title.cpp level.cpp terrain.cpp math.cpp resources.cpp globals.cpp timer.cpp cannon.cpp ball.cpp
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
