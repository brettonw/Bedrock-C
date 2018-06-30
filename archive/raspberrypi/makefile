CC=g++
CFLAGS=-c -Wall -DDEBUG
LDFLAGS=
SOURCES=ReferenceCountedObject.cpp Test.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=rcptr

all: $(SOURCES) $(EXECUTABLE)
	$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
