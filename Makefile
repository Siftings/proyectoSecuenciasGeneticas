# makefile
GPP = g++
FLAGS = -std=c++11 -Wall 

TARGET = bin/programa

SOURCES = main.cpp interfaz.cpp secuencias.cpp huffman.cpp grafo.cpp
OBJECTS = build/main.o build/interfaz.o build/secuencias.o build/huffman.o build/grafo.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p bin
	$(GPP) $(FLAGS) -o $(TARGET) $(OBJECTS)

build/%.o: %.cpp
	@mkdir -p build
	$(GPP) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
